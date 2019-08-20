
gtest_test_sources = files('check-gtest-working-test.cpp')

gtest_test = executable(
    '@0@-gtest_test'.format(meson.project_name()),
                sources: gtest_test_sources,
                 c_args: kanban_app_c_args,
              link_args: kanban_app_ld_args,
           dependencies: [test_deps, kanban_app_deps],
                    pie: true,
    include_directories: config_h_inc
)

test('gtest tests', gtest_test)

