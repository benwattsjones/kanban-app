/* src/presenters/kanban-application.c
 *
 * Copyright (C) 2019 Ben Watts-Jones
 *
 * This file is part of kanban-app.
 *
 * kanban-app is released under the terms of the GNU General Public License,
 * either version 3 of the license, or (at your option) any later version.
 * You should have received a copy of the GNU General Public License along 
 * with kanban-app. If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "kanban-application.h"

#include "kanban-list-store.h"
#include "presenter-view-interface.h"
#include <config.h>

#include <gtk/gtk.h>

#define NULL_ARGUEMENT_PLACEHOLDER NULL
#define NULL_ARG_DATA NULL

enum
{
  CONTINUE_DEFAULT_KANBAN_PROGRAM_CODE = -1,
  SUCCESS_KANBAN_PROGRAM_CODE = 0,
};

struct _KanbanApplication
{
  GtkApplication    parent_instance;
  KanbanListStore  *viewmodel;
};

static GOptionEntry entries[] =
{
  { "version", 'V', G_OPTION_FLAG_NONE,
    G_OPTION_ARG_NONE, NULL_ARG_DATA,
    "Display program version and exit", NULL_ARGUEMENT_PLACEHOLDER },
  { NULL }
};


G_DEFINE_TYPE(KanbanApplication, kanban_application, GTK_TYPE_APPLICATION)


static void
kanban_application_startup (GApplication *app)
{
  KanbanApplication *self = KANBAN_APPLICATION (app);
  self->viewmodel = initialize_viewmodel (1); // TODO: temp col_id implement new object

  G_APPLICATION_CLASS (kanban_application_parent_class)->startup (app);
}

static void
kanban_application_activate (GApplication *app)
{
  initialize_kanban_view (KANBAN_APPLICATION (app));
}

static void
kanban_application_open (GApplication  *app,
                         GFile        **files,
                         gint           n_files,
                         const gchar   *hint)
{
  (void) hint;
  (void) n_files;
  (void) files;
  (void) app;
}

static gint
kanban_application_handle_local_options (GApplication *app,
                                         GVariantDict *options)
{
  (void) app;
  if (g_variant_dict_contains (options, "version"))
    {
      g_print("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
      return SUCCESS_KANBAN_PROGRAM_CODE;
    }
  return CONTINUE_DEFAULT_KANBAN_PROGRAM_CODE;
}

static void
kanban_application_shutdown (GApplication *app)
{
  KanbanApplication *self = KANBAN_APPLICATION (app);
  destroy_viewmodel (self->viewmodel);
  G_APPLICATION_CLASS (kanban_application_parent_class)->shutdown (app);
}

static void
kanban_application_init (KanbanApplication *app)
{
  g_application_set_option_context_parameter_string (G_APPLICATION (app), 
    "- description of program for help");
  g_application_add_main_option_entries (G_APPLICATION (app), entries);
  app->viewmodel = NULL;
}

static void
kanban_application_class_init (KanbanApplicationClass *klass)
{
  GApplicationClass *application_class = G_APPLICATION_CLASS (klass);
  application_class->startup = kanban_application_startup;
  application_class->activate = kanban_application_activate;
  application_class->open = kanban_application_open;
  application_class->handle_local_options = kanban_application_handle_local_options;
  application_class->shutdown = kanban_application_shutdown;
}

KanbanListStore *
kanban_application_get_viewmodel (KanbanApplication *self)
{
  return self->viewmodel;
}

int
initialize_kanban_presenter (int argc, char *argv[])
{
  KanbanApplication *app = g_object_new (KANBAN_APPLICATION_TYPE,
                                         "application-id", APPLICATION_ID,
                                         "flags", G_APPLICATION_HANDLES_OPEN,
                                         NULL);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}

