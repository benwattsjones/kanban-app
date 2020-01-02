
kanban_test_c_args = kanban_app_c_args
kanban_test_c_args += '-DTESTING_ONLY_ACCESS'

# Prevent clang warnings from G_DECLARE_TYPE macros in mocks
if meson.get_compiler('c').get_id() == 'clang'
    kanban_test_c_args += ['-Wno-unused-function']
endif

default_test_args = {
                 'c_args': kanban_test_c_args,
               'cpp_args': kanban_test_c_args,
              'link_args': kanban_app_ld_args,
    'include_directories': [config_h_inc, src_inc],
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

test('application_entry_tests',
    executable('application_entry_tests',
        files('../src/kanban-application.c',
              '../src/presenters/kanban-board-observer-interface.c',
              'application-entry-tests.cpp'),
        kwargs: default_test_args,
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

test('column_viewmodel_tests',
    executable('column_viewmodel_tests',
        files('../src/presenters/kanban-column-viewmodel.c',
              '../src/presenters/kanban-column-observable-interface.c',
              '../src/presenters/kanban-card-viewmodel.c',
              'kanban-column-viewmodel-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('column_observable_interface_tests',
    executable('column_observable_interface_tests',
        files('../src/presenters/kanban-column-observable-interface.c',
              '../src/presenters/kanban-column-viewmodel.c',
              '../src/presenters/kanban-card-viewmodel.c',
              'kanban-column-observable-interface-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('board_presenter_tests',
    executable('board_presenter_tests',
        files('../src/presenters/kanban-board-presenter.c',
              '../src/presenters/kanban-column-viewmodel.c',
              '../src/presenters/kanban-column-observable-interface.c',
              '../src/presenters/kanban-card-viewmodel.c',
              'kanban-board-presenter-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('board_observer_interface_tests',
    executable('board_observer_interface_tests',
        files('../src/presenters/kanban-board-observer-interface.c',
              'kanban-board-observer-interface-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('model_observer_tests',
    executable('model_observer_tests',
        files('../src/presenters/model-observer.c',
              'model-observer-tests.cpp'),
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('presenters_views_integration_tests',
    executable('presenters_views_integration_tests',
        files('../src/presenters/kanban-card-viewmodel.c',
              '../src/presenters/kanban-column-viewmodel.c',
              '../src/presenters/kanban-column-observable-interface.c',
              '../src/presenters/kanban-board-presenter.c',
              '../src/presenters/kanban-board-observer-interface.c',
              '../src/presenters/model-observer.c',
              '../src/views/kanban-column-view.c',
              '../src/views/kanban-board-view.c',
              '../src/kanban-utils.c',
              'presenters-views-intergration-tests.cpp'),
        kanban_app_gresources,
        kwargs: default_test_args,
    ),
    is_parallel: false,
    suite: 'integration',
)

test('board_view_tests',
    executable('board_view_tests',
        files('../src/views/kanban-board-view.c',
              '../src/presenters/kanban-column-observable-interface.c',
              '../src/presenters/kanban-board-observer-interface.c',
              'kanban-board-view-tests.cpp'),
        kanban_app_gresources,
        kwargs: default_test_args,
    ),
    suite: 'unit',
)

test('column_view_tests',
    executable('column_view_tests',
        files('../src/views/kanban-column-view.c',
              '../src/presenters/kanban-column-viewmodel.c',
              '../src/presenters/kanban-card-viewmodel.c',
              '../src/presenters/kanban-column-observable-interface.c',
              '../src/kanban-utils.c',
              'kanban-column-view-tests.cpp'),
        kanban_app_gresources,
        kwargs: default_test_args,
    ),
    suite: 'unit',
)


add_test_setup('valgrind',
    exe_wrapper: ['valgrind', '--leak-check=full', '--suppressions=/usr/share/glib-2.0/valgrind/glib.supp', '--error-exitcode=1', '--show-leak-kinds=definite,indirect', '--trace-children=yes', '--errors-for-leak-kinds=definite,indirect'],
)
