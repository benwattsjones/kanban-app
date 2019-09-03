

default_test_args = {
                 'c_args': [kanban_app_c_args, '-DTESTING_ONLY_ACCESS'],
               'cpp_args': ['-DTESTING_ONLY_ACCESS'],
              'link_args': kanban_app_ld_args,
           'dependencies': [test_deps, kanban_app_deps],
                    'pie': true,
}


test('gtest_tests',
    executable('gtest_tests',
        files('check-gtest-working-test.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('kanban_application_tests',
    executable('kanban_application_tests',
        files('../src/kanban-application.c',
              'application-entry-tests.cpp'),
        kwargs: default_test_args,
        include_directories: config_h_inc,
    ),
    is_parallel: false,
    suite: 'integration',
)

test('card_viewmodel_tests', 
    executable('card_viewmodel_tests', 
        files('../src/presenters/kanban-card-viewmodel.c',
              'kanban-card-viewmodel-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('list_store_tests',
    executable('list_store_tests',
        files('../src/presenters/kanban-list-store.c',
              '../src/presenters/kanban-card-viewmodel.c',
              'kanban-list-store-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('column_store_tests',
    executable('column_store_tests',
        files('../src/presenters/kanban-column-store.c',
              '../src/presenters/kanban-list-store.c',
              '../src/presenters/kanban-card-viewmodel.c',
              'kanban-column-store-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('presenter_integration_tests',
    executable('presenter_integration_tests',
        files('../src/kanban-application.c',
              '../src/presenters/kanban-card-viewmodel.c',
              '../src/presenters/kanban-list-store.c',
              '../src/presenters/kanban-column-store.c',
              'presenters-cards-intergration-tests.cpp'),
        kwargs: default_test_args,
        include_directories: config_h_inc,
    ),
    is_parallel: false,
    suite: 'integration',
)


add_test_setup('valgrind',
    exe_wrapper: ['valgrind', '--leak-check=full', '--suppressions=/usr/share/glib-2.0/valgrind/glib.supp', '--error-exitcode=1', '--show-leak-kinds=all', '--trace-children=yes', '--errors-for-leak-kinds=definite,indirect', '-s'],
)

