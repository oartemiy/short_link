#include "userver/server/handlers/http_handler_base.hpp"
#include <userver/components/component.hpp>
#include "userver/storages/postgres/postgres_fwd.hpp"

namespace short_link::handlers {

class Redirect final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-redirect";

    Redirect(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& component_context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context) const override;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};
}  // namespace short_link::handlers
