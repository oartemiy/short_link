import pytest

from testsuite.databases.pgsql import discover 

pytest_plugins = [
    'pytest_userver.plugins.core',
    'pytest_userver.plugins.postgresql', 
]

@pytest.fixture(scope='session')
def initial_data_path(service_source_dir):
    """Path for find files with data"""
    return [
        service_source_dir / 'postgresql/data',
    ]


@pytest.fixture(scope='session')
def pgsql_local(service_source_dir, pgsql_local_create):
    """Create schemas databases for tests"""
    databases = discover.find_schemas(
        'short_link',
        [service_source_dir.joinpath('postgresql/schemas')],
    )
    return pgsql_local_create(list(databases.values()))


# import pytest

# pytest_plugins = [
#     'pytest_userver.plugins.core',
#     'pytest_userver.plugins.postgresql',
# ]

# PG_DSN = 'postgresql://oartemiy@localhost:5432/short_link'


# @pytest.fixture(scope='session')
# def pgsql_local(pgsql_local_create):
#     return pgsql_local_create([])


# @pytest.fixture(scope='session')
# def userver_pg_config():
#     def _patch_config(config_yaml, config_vars):
#         components = config_yaml['components_manager']['components']
#         for name, params in components.items():
#             if params and ('dbconnection' in params or 'dbconnection#env' in params):
#                 params['dbconnection'] = PG_DSN

#     return _patch_config