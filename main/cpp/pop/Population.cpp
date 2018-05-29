/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Core Population class
 */

#include "Population.h"
#include <pool/ContactPoolType.h>

#include "behaviour/belief_policies/Imitation.h"
#include "behaviour/belief_policies/NoBelief.h"
#include "pop/DefaultPopBuilder.h"
#include "pop/GenPopBuilder.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/RNManager.h"
#include "util/RunConfigManager.h"

using namespace boost::property_tree;
using namespace std;
using namespace stride::util;

namespace stride {

std::shared_ptr<Population> Population::Create(const boost::property_tree::ptree& configPt)
{
        // --------------------------------------------------------------
        // Create (empty) population & and give it a ContactLogger.
        // --------------------------------------------------------------
        struct make_shared_enabler : public Population
        {
        };
        auto pop = make_shared<make_shared_enabler>();
        if (configPt.get<bool>("run.contact_output_file", true)) {
                const auto prefix       = configPt.get<string>("run.output_prefix");
                const auto logPath      = FileSys::BuildPath(prefix, "contact_log.txt");
                pop->GetContactLogger() = LogUtils::CreateRotatingLogger("contact_logger", logPath.string());
                pop->GetContactLogger()->set_pattern("%v");
        } else {
                pop->GetContactLogger() = LogUtils::CreateNullLogger("contact_logger");
        }

        pop->m_belief_pt = configPt.get_child("run.belief_policy");

        // ------------------------------------------------
        // Setup RNManager.
        // ------------------------------------------------
        RNManager rnManager(RNManager::Info{configPt.get<string>("pop.rng_type", "lcg64"),
                                            configPt.get<unsigned long>("pop.rng_seed", 101UL), "",
                                            configPt.get<unsigned int>("run.num_threads")});

        auto stride_logger = spdlog::get("stride_logger");

        boost::optional<const ptree&> regions = configPt.get_child_optional("run.regions");

        // -----------------------------------------------------------------------------------------
        // Build population for each region
        // -----------------------------------------------------------------------------------------
        if (!regions) {
                pop->m_regions["Default"] = 0;

                std::string geopop_type = configPt.get<std::string>("run.geopop_type", "default");

                CreateRegion(geopop_type, configPt, configPt.get_child("run"), pop, "Default", rnManager);
        } else {
                std::size_t currentId = 0;
                for (const auto& region : configPt.get_child("run.regions")) {
                        std::string name     = region.second.get<std::string>("name");
                        pop->m_regions[name] = currentId;
                        currentId++;
                }
                for (const auto& region : configPt.get_child("run.regions")) {
                        std::string name        = region.second.get<std::string>("name");
                        std::string geopop_type = region.second.get<std::string>("geopop_type", "default");

                        CreateRegion(geopop_type, configPt, region.second, pop, name, rnManager);
                }
        }

        return pop;
}

std::shared_ptr<Population> Population::Create(const string& configString)
{
        return Create(RunConfigManager::FromString(configString));
}

std::shared_ptr<Population> Population::Create()
{
        // --------------------------------------------------------------
        // Create (empty) population and return it
        // --------------------------------------------------------------
        struct make_shared_enabler : public Population
        {
        };
        auto r = make_shared<make_shared_enabler>();
        r->m_belief_pt.add("name", "NoBelief");
        r->m_regions["Default"] = 0;
        return r;
}

unsigned int Population::GetAdoptedCount() const
{
        unsigned int total{0U};
        for (const auto& p : *this) {
                if (p.GetBelief()->HasAdopted()) {
                        total++;
                }
        }
        return total;
}

unsigned int Population::GetInfectedCount() const
{
        unsigned int total{0U};
        for (const auto& p : *this) {
                const auto& h = p.GetHealth();
                total += h.IsInfected() || h.IsRecovered();
        }
        return total;
}

void Population::CreatePerson(std::size_t regionId, unsigned int id, double age, unsigned int householdId,
                              unsigned int k12SchoolId, unsigned int college, unsigned int workId,
                              unsigned int primaryCommunityId, unsigned int secondaryCommunityId)
{
        if (m_currentRegionId != regionId) {
                // TODO this is only executed for the not latest regions
                assert(regionId > m_currentRegionId);
                m_regionRanges.SetRange(m_currentStart, m_currentRegionId);
                m_currentRegionId = regionId;
                m_currentStart    = size() - 1;
        }

        emplace_back(id, age, householdId, k12SchoolId, college, workId, primaryCommunityId, secondaryCommunityId,
                     regionId);
}

const std::unordered_map<std::string, std::size_t>& Population::GetRegionIdentifiers() const { return m_regions; }

ContactPool* Population::CreateContactPool(std::size_t regionId, ContactPoolType::Id typeId)
{
        assert(regionId >= m_currentRegionId);

        m_pool_sys[typeId].emplace_back(m_currentContactPoolId++, typeId);

        auto r = &m_pool_sys[typeId].back();

        if (typeId == ContactPoolType::Id::Work) {
                m_work[regionId] = r;
        } else if (typeId == ContactPoolType::Id::PrimaryCommunity) {
                m_primaryCommunities[regionId] = r;
        }

        return r;
}

void Population::CreateRegion(const std::string& geopop_type, const boost::property_tree::ptree& configPt,
                              const boost::property_tree::ptree& regionPt, const std::shared_ptr<Population>& pop,
                              const std::string& name, stride::util::RNManager& rnManager)
{
        auto stride_logger = spdlog::get("stride_logger");

        if (geopop_type == "import") {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with imported pop.", name);
                ImportPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        } else if (geopop_type == "generate") {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with generated pop.", name);
                GenPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        } else {
                if (stride_logger)
                        stride_logger->info("Creating region \"{}\" with Default pop.", name);
                DefaultPopBuilder(configPt, regionPt, rnManager).Build(pop, pop->m_regions[name]);
        }
}
} // namespace stride
