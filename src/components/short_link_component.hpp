#pragma once

#include <chrono>
#include <string_view>
#include <userver/components/component_base.hpp>
#include <userver/utils/periodic_task.hpp>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include <userver/storages/postgres/cluster.hpp>
#include "storages/link_storage.hpp"

namespace short_link::components {

inline constexpr std::chrono::minutes kPeriod{std::chrono::minutes(3)};

using userver::components::ComponentConfig;
using userver::components::ComponentContext;
using userver::components::LoggableComponentBase;
using userver::utils::PeriodicTask;

class ShortLinkComponent final : public LoggableComponentBase {
public:
    static constexpr std::string_view kName = "short-link-component";

    ShortLinkComponent(const ComponentConfig& config, const ComponentContext& component_context);

    const short_link::storages::LinkStorage& GetLinkStorageRef() const {
        return link_storage_;
    }

private:
    void ClearLinks() const;
    short_link::storages::LinkStorage link_storage_;
    PeriodicTask periodic_task_;
};

}  // namespace short_link::components
