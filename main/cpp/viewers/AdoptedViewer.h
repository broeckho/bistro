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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Observer for Adopted output.
 */

#include "output/AdoptedFile.h"
#include "sim/event/Payload.h"

#include <iostream>
#include <spdlog/spdlog.h>

namespace stride {
namespace viewers {

/// Viewer of Simulator for cases output.
class AdoptedViewer
{
public:
        /// Instantiate cases viewer.
        AdoptedViewer(const std::string& output_prefix) : m_adopted(), m_adopted_file(output_prefix) {}

        /// Let viewer perform update.
        void update(const sim_event::Payload& p);

private:
        std::vector<unsigned int> m_adopted;
        output::AdoptedFile       m_adopted_file;
};

} // namespace viewers
} // namespace stride
