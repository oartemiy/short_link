#include "redirect_handler.hpp"
#include <userver/http/common_headers.hpp>

#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/server/http/http_status.hpp>

using userver::components::ComponentConfig;
using userver::components::ComponentContext;
using userver::server::handlers::HttpHandlerBase;

namespace short_link::handlers {

Redirect::Redirect(const ComponentConfig& config, const ComponentContext& component_context)
    : HttpHandlerBase(config, component_context),
      pg_cluster_(
          component_context.FindComponent<userver::components::Postgres>("postgres-db-links")
              .GetCluster()) {
}

std::string Redirect::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& context) const {
    auto code = request.GetPathArg("code");

    auto res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE short_link_schema.links SET clicks = clicks + 1 WHERE code = $1 "
        "RETURNING original_url",
        code);

    auto original_url = res[0][0].As<std::string>();

    request.GetHttpResponse().SetHeader(userver::http::headers::kLocation, original_url);
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kPermanentRedirect);

    return "";
}
}  // namespace short_link::handlers
