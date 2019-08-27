

default_test_args = {
                 'c_args': kanban_app_c_args,
              'link_args': kanban_app_ld_args,
           'dependencies': [test_deps, kanban_app_deps],
                    'pie': true,
}


test('gtest_tests',
    executable('gtest_tests',
        files('check-gtest-working-test.cpp'),
        kwargs: default_test_args,
    )
)

test('kanban_application_tests',
    executable('kanban_application_tests',
        files('../src/presenters/kanban-application.c',
              'application-entry-tests.cpp'),
        kwargs: default_test_args,
        include_directories: config_h_inc,
    )
)

test('card_viewmodel_tests', 
    executable('card_viewmodel_tests', 
        files('../src/presenters/kanban-card-viewmodel.c',
              'kanban-card-viewmodel-tests.cpp'),
        kwargs: default_test_args,
    )
)

add_test_setup('valgrind',
    exe_wrapper: ['valgrind', '--leak-check=full', '--suppressions=/usr/share/glib-2.0/valgrind/glib.supp', '--error-exitcode=1', '--show-leak-kinds=all', '--trace-children=yes', '--errors-for-leak-kinds=definite,indirect', '-s'],
)

