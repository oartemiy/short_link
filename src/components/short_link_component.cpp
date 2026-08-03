#include "short_link_component.hpp"
#include <chrono>
#include "userver/utils/periodic_task.hpp"
#include <userver/logging/log.hpp>

namespace short_link::components {

ShortLinkComponent::ShortLinkComponent(const ComponentConfig& config,
                                       const ComponentContext& component_context)
    : LoggableComponentBase(config, component_context),
      link_storage_(component_context),
      periodic_task_("clear-links", PeriodicTask::Settings(kPeriod), [this]() { ClearLinks(); }) {
}

void ShortLinkComponent::ClearLinks() const {
    LOG_INFO() << "Cleaning expired links...\n";
    link_storage_.CleanupExpiredLinks();
    LOG_INFO() << "Cleaning expired links finished\n";
}

}  // namespace short_link::components
