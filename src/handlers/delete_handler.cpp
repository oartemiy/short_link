#include "delete_handler.hpp"
#include "components/short_link_component.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"

namespace short_link::handlers {

Delete::Delete(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& component_context)
    : userver::server::handlers::HttpHandlerJsonBase(config, component_context),
      link_storage_(
          component_context
              .FindComponent<short_link::components::ShortLinkComponent>("short-link-component")
              .GetLinkStorageRef()) {
}

Delete::Value Delete::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request, [[maybe_unused]] const Value& request_json,
    [[maybe_unused]] userver::server::request::RequestContext& context) const {
    const std::string& code = request.GetPathArg("code");
    auto res = link_storage_.DeleteCode(code);
    auto json = userver::formats::json::ValueBuilder{};
    if (res) {
        json["status"] = "deleted";
        json["code"] = code;
    } else {
        json["error"] = "Short code " + code + " has expired or was not created";
        request.GetHttpResponse().SetStatusNotFound();
    }
    auto json_res = json.ExtractValue();
    return json_res;
}

}  // namespace short_link::handlers
