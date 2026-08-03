#include "shorten_handler.hpp"

#include <userver/storages/postgres/component.hpp>
#include "components/short_link_component.hpp"

namespace short_link::handlers {

Shorten::Shorten(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& component_context)
    : HttpHandlerJsonBase(config, component_context),
      link_storage_ref_(
          component_context
              .FindComponent<short_link::components::ShortLinkComponent>("short-link-component")
              .GetLinkStorageRef()) {
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

    auto result = link_storage_ref_.InsertLink(original_url);

    auto json_result = result.ToJSON();
    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);
    return json_result;
}

}  // namespace short_link::handlers
