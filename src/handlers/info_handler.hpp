#pragma once

#include <string_view>
#include "storages/link_storage.hpp"
#include "userver/components/component_context.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"

namespace short_link::handlers {

class Info final: public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-info";

    Info(const userver::components::ComponentConfig& config,
         const userver::components::ComponentContext& component_context);

    Value HandleRequestJsonThrow(const HttpRequest& request, const Value& request_json,
                                 RequestContext& context) const override;

private:
    const short_link::storages::LinkStorage& link_storage_ref_;
};

}  // namespace short_link::handlers
