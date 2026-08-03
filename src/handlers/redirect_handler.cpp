#include "redirect_handler.hpp"
#include <userver/http/common_headers.hpp>

#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/server/http/http_status.hpp>
#include "components/short_link_component.hpp"

using userver::components::ComponentConfig;
using userver::components::ComponentContext;
using userver::server::handlers::HttpHandlerBase;

namespace short_link::handlers {

Redirect::Redirect(const ComponentConfig& config, const ComponentContext& component_context)
    : HttpHandlerBase(config, component_context),
      link_storage_ref_(
          component_context
              .FindComponent<short_link::components::ShortLinkComponent>("short-link-component")
              .GetLinkStorageRef()) {
}

std::string Redirect::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& context) const {
    const auto& code = request.GetPathArg("code");

    auto result = link_storage_ref_.Redirect(code);

    if (result.has_value()) {
        request.GetHttpResponse().SetHeader(userver::http::headers::kLocation, *result);
        request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kPermanentRedirect);
    } else {
        request.GetHttpResponse().SetStatusNotFound();
        request.GetHttpResponse().SetContentType("text/plain; charset=utf-8");
        return "Short link with code: \"" + code + "\" not found";
    }

    return "";
}
}  // namespace short_link::handlers
