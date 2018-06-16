#pragma once

#include "ContactPool.h"
#include <spdlog/logger.h>

#include <queue>

namespace stride {

/**
 * Stores the information about travells. One TravellerIndex stores information for a specific region. (i.e. all persons
 * stored are from the same region).
 * To have the code which handles the start and return of a person in the same location this class also handles that.
 */
class TravellerIndex
{
public:
        /// Default constructor
        TravellerIndex();

        /// Stores the information when the travel is started. F
        /// \p from is the original contactPool (type is \p type)
        /// \p to is the foreign contactPool (same type as from)
        /// \p leaveDay is the day at which the person will return
        /// \p type is the type of the travel Work or PrimaryCommunity (i.e. recreational)
        void StartTravel(unsigned int from, ContactPool* to, Person*, std::size_t leaveDay, ContactPoolType::Id type);

        /// Returns the travels at the given day
        void ReturnTravelers(std::size_t currentDay);

private:
        /**
         * Information stored for a travel.
         */
        struct TravellerInfo
        {
                unsigned int        from;
                ContactPool*        to;
                Person*             person;
                std::size_t         leaveDay;
                ContactPoolType::Id type;
                bool                operator>(const TravellerInfo& other) const { return leaveDay > other.leaveDay; }
        };

        std::shared_ptr<spdlog::logger> m_travelLogger; ///< Logger to which the travel information will be logged

        std::priority_queue<TravellerInfo, std::vector<TravellerInfo>, std::greater<>> m_data; ///< min-heap to
                                                                                               ///< efficiently store
                                                                                               ///< the persons which
                                                                                               ///< are travelling.
                                                                                               ///< Indexed by the day
                                                                                               ///< they return (lowest
                                                                                               ///< first).
};

} // namespace stride
