#pragma once

#include <string>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include "models/link_info.hpp"
#include "userver/components/component_context.hpp"
#include <userver/storages/postgres/cluster.hpp>

namespace short_link::storages {

using models::LinkInfo;
using userver::components::ComponentContext;

class LinkStorage {
public:
    static constexpr int kDefaultTtlSeconds = 5 * 60;

    explicit LinkStorage(const ComponentContext& component_context);

    [[nodiscard]] LinkInfo InsertLink(const std::string& original_url) const;

    [[nodiscard]] std::optional<LinkInfo> GetCodeInfo(const std::string& code) const;

    [[nodiscard]] std::optional<std::string> Redirect(const std::string& code) const;

    [[nodiscard]] bool DeleteCode(const std::string& code) const;

    void CleanupExpiredLinks() const;

private:
    bool IsCodeAvailable(const std::string& code) const;

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace short_link::storages
