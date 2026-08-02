#include "shorten_handler.hpp"

#include <userver/storages/postgres/component.hpp>
#include "impl/generate_code.hpp"

namespace short_link::handlers {

Shorten::Shorten(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      pg_cluster_(
          component_context.FindComponent<userver::components::Postgres>("postgres-db-links")
              .GetCluster()) {
}

Value Shorten::HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json,
                                      [[maybe_unused]] RequestContext& context) const {
    const auto original_url = request_json["url"].As<std::string>();
    if ((!original_url.starts_with("http://") && !original_url.starts_with("https://")) ||
        original_url.empty()) {
        userver::formats::json::ValueBuilder error;
        error["error"] = "url: " + original_url + " is not valid!";
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kBadRequest);
        return error.ExtractValue();
    }

    std::string code;
    constexpr auto kDefaultTtlSeconds = 60 * 5;

    do {
        code = short_link::impl::GenerateCode();
    } while (!pg_cluster_
                  ->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                            "SELECT code FROM short_link_schema.links WHERE code = $1", code)
                  .IsEmpty());
    auto res =
        pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                             "INSERT INTO short_link_schema.links(code, original_url, created_at, "
                             "expires_at) "
                             "VALUES($1, $2, NOW(), NOW() + make_interval(secs => $3)) "
                             "RETURNING json_build_object("
                             "  'code', code,"
                             "  'short_url', 'http://localhost:8080/' || code,"
                             "  'original_url', original_url,"
                             "  'created_at', created_at,"
                             "  'expires_at', expires_at"
                             ")",
                             code, original_url, kDefaultTtlSeconds);

    auto json_result = res[0][0].As<userver::formats::json::Value>();
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);
    return json_result;
}

}  // namespace short_link::handlers
