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
 *  Copyright 2017, 2018 Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Implementation of Measles runs for benchmarking.
 */

#include "myhayai/BenchmarkRunner.hpp"
#include "pop/Population.h"
#include "sim/SimRunner.h"
#include "util/RunConfigManager.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace myhayai;
using boost::property_tree::ptree;

void MeaslesBench()
{
        auto builder = [](unsigned int n) {
                auto configPt = make_shared<ptree>(RunConfigManager::Create("BenchMeasles"));
                return [n, configPt]() {
                        return Test([n, configPt]() {
                                configPt->put("run.num_threads", n);
                                RNManager rn_manager;
                                rn_manager.Initialize(RNManager::Info(configPt->get<string>("run.rng_type", "mrg2"),
                                                                      configPt->get<unsigned long>("run.rng_seed", 1UL),
                                                                      configPt->get<string>("run.rng_state", ""),
                                                                      configPt->get<unsigned int>("run.num_threads")));
                                SimRunner(*configPt, Population::Create(*configPt, rn_manager), rn_manager).Run();
                        });
                };
        };

        const auto num = RunConfigManager::CreateNumThreads();
        for (const auto n : num) {
                auto info = [n]() { return ptree().put("num_threads", n); };
                BenchmarkRunner::RegisterTest("Measles", "NumThreads." + ToString(n), 7, builder(n), info);
        }
}
