#pragma once
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
 *  Copyright 2017, 2018 Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Initialize populations.
 */

#include "AbstractPopBuilder.h"
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>

namespace stride {

class Population;
namespace util {
class RNManager;
}

/**
 * Initializes Population objects.
 */
class DefaultPopBuilder : public AbstractPopBuilder
{
public:
        using AbstractPopBuilder::AbstractPopBuilder;

        /// Build Population and return it afterwards.
        /// The steps are:
        /// - Check input data.
        /// - Read persons from file and instatiate them.
        /// - Fill up the various type of contactpools.
        /// - Seed the population with contact survey participants.
        std::shared_ptr<Population> Build(std::shared_ptr<Population> pop, std::size_t regionId,
                                          std::string regionName);

private:
        /// Generates pop's individuals and return pop.
        std::shared_ptr<Population> MakePersons(std::shared_ptr<Population> pop);
        std::size_t                 m_regionId = 0; ///< RegionId, used to create persons
};

} // namespace stride
