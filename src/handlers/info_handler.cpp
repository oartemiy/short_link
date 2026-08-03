#include "info_handler.hpp"
#include "components/short_link_component.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"

namespace short_link::handlers {

Info::Info(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
    : userver::server::handlers::HttpHandlerJsonBase(config, component_context),
      link_storage_ref_(
          component_context
              .FindComponent<short_link::components::ShortLinkComponent>("short-link-component")
              .GetLinkStorageRef()) {
}

Info::Value Info::HandleRequestJsonThrow(const HttpRequest& request,
                                         [[maybe_unused]] const Value& request_json,
                                         [[maybe_unused]] RequestContext& context) const {
    const auto& code = request.GetPathArg("code");

    auto info = link_storage_ref_.GetCodeInfo(code);
    Value result;
    if (info.has_value()) {
        result = info->ToJSON();
    } else {
        userver::formats::json::ValueBuilder json;
        json["error"] = "Short link with code: \"" + code + "\" not found";
        result = json.ExtractValue();
    }
    return result;
}

}  // namespace short_link::handlers
