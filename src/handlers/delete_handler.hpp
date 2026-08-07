#pragma once

#include <string_view>
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"
#include "userver/server/http/http_request.hpp"
#include "userver/server/request/request_context.hpp"
#include "storages/link_storage.hpp"

namespace short_link::handlers {

class Delete final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-delete";

    Delete(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context);

    Value HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                 const Value& request_json,
                                 userver::server::request::RequestContext& context) const override;

private:
    const short_link::storages::LinkStorage& link_storage_;
};

}  // namespace short_link::handlers
