project(
       'kanban-app',
       ['c', 'cpp'],
    default_options: ['c_std=gnu11', 'cpp_std=c++11'],
            license: 'GPL-3.0-or-later',
      meson_version: '>=0.50.0',
            version: '0.0.1'
)

config_h_data = configuration_data()

config_h_defines = [
    ['PACKAGE_NAME',     meson.project_name()],
    ['PACKAGE_VERSION',  meson.project_version()],
    ['PACKAGE_URL',     'https://github.com/benwattsjones/kanban-app'],
    ['APPLICATION_ID',  'com.benwattsjones.kanban']
]

foreach define: config_h_defines
    config_h_data.set_quoted(define[0], define[1])
endforeach

configure_file(
           output: 'config.h',
    configuration: config_h_data
)

config_h_inc = include_directories('.')

gnome = import('gnome')

kanban_app_sources = files(
    'src/presenters/kanban-application.c',
    'src/presenters/model-observer.c',
    'src/presenters/kanban-list-store.c',
    'src/presenters/kanban-card-viewmodel.c',
    'src/views/kanban-window.c',
    'src/views/kanban-list-box.c',
    'src/models/model-presenter-interface.c',
    'src/models/kanban-cards.c',
)

kanban_app_sources += gnome.compile_resources(
    'kanban-app-resources',
    'src/views/kanban-app.gresource.xml',
    source_dir: 'src/views',
    export: true
)

kanban_app_sources_main = kanban_app_sources + files('src/main.c')

kanban_app_c_args = [
    '-fexceptions',
    '-fstack-clash-protection',
    '-fstack-protector-strong',
    '-fasynchronous-unwind-tables',
    '-fpic',
    '-shared',
    '-fcf-protection',
    '-pipe',
    '-g',
    '-O3',
    '-march=native',
    '-D_FORTIFY_SOURCE=2',
    '-Wall',
    '-Wextra',
    '-Werror=format-security',
    '-Werror=implicit-function-declaration',
    '-Wpedantic'
]

kanban_app_ld_args = [
    '-Wl,-z,defs',
    '-Wl,-z,now',
    '-Wl,-z,relro'
]

kanban_app_deps = [
    dependency('gtk+-3.0', version: '>=3.24.10')
] 

executable(
    meson.project_name(),
                 sources: kanban_app_sources_main,
                  c_args: kanban_app_c_args,
               link_args: kanban_app_ld_args,
            dependencies: kanban_app_deps,
                     pie: true,
     include_directories: config_h_inc
)

gtest_dep = dependency('gtest_main', main: true, required: false)

if gtest_dep.found()
    test_sources = kanban_app_sources
    test_sources += files(
        'tests/check-gtest-working-test.cpp'
    )

    e = executable(
        '@0@-tests'.format(meson.project_name()),
                    sources: test_sources,
                     c_args: kanban_app_c_args,
                  link_args: kanban_app_ld_args,
               dependencies: [gtest_dep, kanban_app_deps],
                        pie: true,
        include_directories: config_h_inc
    )

    test('gtest tests', e)
else
    warning('gtest dependency not found. NOT building tests.')
    message('Consider installing gtest: dnf install gtest-devel')
endif

