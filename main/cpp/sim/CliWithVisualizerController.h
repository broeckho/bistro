#pragma once

#include "CliController.h"

namespace stride {
class CliWithVisualizerController : public CliController
{
public:
        CliWithVisualizerController(const boost::property_tree::ptree& config_pt) : CliController(config_pt){};

protected:
        /// Register the viewers of the SimRunner.
        void RegisterViewers(std::shared_ptr<SimRunner> runner) override;
};
} // namespace stride