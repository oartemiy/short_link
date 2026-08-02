#include "short_link_component.hpp"
#include <chrono>
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/utils/periodic_task.hpp"
#include <userver/logging/log.hpp>

namespace short_link::components {

ShortLinkComponent::ShortLinkComponent(const ComponentConfig& config,
                                       const ComponentContext& component_context)
    : LoggableComponentBase(config, component_context),
      pg_cluster_(
          component_context.FindComponent<userver::components::Postgres>("postgres-db-links")
              .GetCluster()),
      periodic_task_("clear-links", PeriodicTask::Settings(kPeriod), [this]() { ClearLinks(); }) {
}

void ShortLinkComponent::ClearLinks() const {
    LOG_INFO() << "Cleaning expired links...\n";
    pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                         "DELETE FROM short_link_schema.links WHERE expires_at <= NOW()");

    LOG_INFO() << "Cleaning expired links finished\n";
}

}  // namespace short_link::components
