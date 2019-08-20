
gtest_test_sources = files('check-gtest-working-test.cpp')

gtest_test = executable(
    '@0@-gtest_test'.format(meson.project_name()),
                sources: gtest_test_sources,
                 c_args: kanban_app_c_args,
              link_args: kanban_app_ld_args,
           dependencies: [test_deps, kanban_app_deps],
                    pie: true,
)

test('gtest tests', gtest_test)


kanban_test_sources = files(
    '../src/presenters/kanban-application.c',
    'application-entry-tests.cpp'
)

kanban_test = executable(
    '@0@-kanban_test'.format(meson.project_name()),
                sources: kanban_test_sources,
                 c_args: kanban_app_c_args,
              link_args: kanban_app_ld_args,
           dependencies: [test_deps, kanban_app_deps],
                    pie: true,
    include_directories: config_h_inc
)

test('kanban tests', kanban_test)

