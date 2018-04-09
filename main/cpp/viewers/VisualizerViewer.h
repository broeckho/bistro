#pragma once

#include <spdlog/logger.h>
#include <sim/event/Payload.h>

namespace stride {
    namespace viewers {
        class VisualizerViewer {
public:
        /// Instantiate cli viewer.
        VisualizerViewer(std::shared_ptr<spdlog::logger> logger) : m_logger(logger) {}

        /// Let viewer perform update.
        void update(const sim_event::Payload& p);

private:
        std::shared_ptr<spdlog::logger> m_logger;
        };
    }
}


