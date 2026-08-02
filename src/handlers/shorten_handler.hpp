#pragma once

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>

#include <userver/storages/postgres/cluster.hpp>

namespace short_link::handlers {

using Value = userver::formats::json::Value;
using HttpRequest = userver::server::http::HttpRequest;
using RequestContext = userver::server::request::RequestContext;
using userver::server::handlers::ExceptionWithCode;

class Shorten final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-shorten";
    Shorten(const userver::components::ComponentConfig&,
            const userver::components::ComponentContext&);

    Value HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json,
                                 RequestContext& context) const override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace short_link::handlers
