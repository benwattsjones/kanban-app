project(
       'kanban-app',
                'c',
    default_options: ['c_std=gnu11'],
            license: 'GPL-3.0-or-later',
      meson_version: '>=0.50.0',
            version: '0.0.1'
)

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
    'src/main.c'
)

kanban_app_sources += gnome.compile_resources(
    'kanban-app-resources',
    'src/views/kanban-app.gresource.xml',
    source_dir: 'src/views',
    export: true
)

kanban_app_c_args = ['-fexceptions',
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
                     '-Wpedantic']

kanban_app_ld_args = ['-Wl,-z,defs',
                      '-Wl,-z,now',
                      '-Wl,-z,relro']

kanban_app_deps = [dependency('gtk+-3.0', version: '>=3.24.10')] 

executable(
    meson.project_name(),
                 sources: kanban_app_sources,
                  c_args: kanban_app_c_args,
               link_args: kanban_app_ld_args,
            dependencies: kanban_app_deps,
                     pie: true
)

