#include "link_storage.hpp"
#include "userver/storages/postgres/component.hpp"
#include "impl/generate_code.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <userver/storages/postgres/cluster_types.hpp>

namespace short_link::storages {

LinkStorage::LinkStorage(const ComponentContext& component_context)
    : pg_cluster_(
          component_context.FindComponent<userver::components::Postgres>("postgres-db-links")
              .GetCluster()) {
}

bool LinkStorage::IsCodeAvailable(const std::string& code) const {
    return pg_cluster_
        ->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                  "SELECT code FROM short_link_schema.links WHERE code = $1", code)
        .IsEmpty();
}

LinkInfo LinkStorage::InsertLink(const std::string& original_url) const {
    std::string code;
    do {
        code = impl::GenerateCode();
    } while (!IsCodeAvailable(code));
    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO short_link_schema.links (code, original_url, created_at, expires_at)"
        "VALUES ($1, $2, NOW(), NOW() + make_interval(secs => $3))"
        "RETURNING"
        "  created_at, "
        "  expires_at; ",
        code, original_url, kDefaultTtlSeconds);
    auto short_url = "http://localhost:8080/" + code;
    auto created_at = res[0][0].As<std::chrono::time_point<std::chrono::system_clock>>();
    auto expires_at = res[0][1].As<std::chrono::time_point<std::chrono::system_clock>>();
    auto info = LinkInfo{code, short_url, original_url, created_at, expires_at, 0};
    return info;
}

std::optional<std::string> LinkStorage::Redirect(const std::string& code) const {
    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE short_link_schema.links SET clicks = clicks + 1 WHERE code = $1 "
        "RETURNING original_url",
        code);
    std::optional<std::string> original_url_opt{std::nullopt};
    if (!res.IsEmpty()) {
        original_url_opt = res[0][0].As<std::string>();
    }
    return original_url_opt;
}

void LinkStorage::CleanupExpiredLinks() const {
    pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                         "DELETE FROM short_link_schema.links WHERE expires_at <= NOW()");
}

std::optional<LinkInfo> LinkStorage::GetCodeInfo(const std::string& code) const {
    auto res = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                    "SELECT original_url, created_at, expires_at, clicks FROM "
                                    "short_link_schema.links WHERE code = $1",
                                    code);
    std::optional<LinkInfo> link_info_opt{std::nullopt};
    if (!res.IsEmpty()) {
        auto short_url = "http://localhost:8080/" + code;
        auto original_url = res[0][0].As<std::string>();
        auto created_at = res[0][1].As<std::chrono::time_point<std::chrono::system_clock>>();
        auto expires_at = res[0][2].As<std::chrono::time_point<std::chrono::system_clock>>();
        auto clicks = res[0][3].As<int>();
        link_info_opt = LinkInfo{code, short_url, original_url, created_at, expires_at, clicks};
    }
    return link_info_opt;
}

}  // namespace short_link::storages
