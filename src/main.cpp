#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include <userver/storages/postgres/component.hpp>

#include <userver/utils/daemon_run.hpp>

#include "handlers/info_handler.hpp"
#include "handlers/shorten_handler.hpp"
#include "handlers/redirect_handler.hpp"
#include "components/short_link_component.hpp"

int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .AppendComponentList(userver::clients::http::ComponentList())
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::congestion_control::Component>()
            .Append<userver::components::Postgres>("postgres-db-links")

            .Append<short_link::components::ShortLinkComponent>()  // "short-link-component"

            .Append<short_link::handlers::Info>()
            .Append<short_link::handlers::Shorten>()
            .Append<short_link::handlers::Redirect>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
