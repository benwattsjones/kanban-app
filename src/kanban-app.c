/* src/kanban-app.c
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

#include <gtk/gtk.h>

#include "kanban-app.h"

#include "kanban-app-win.h"


#define NO_ARGUEMENT_PLACEHOLDER NULL
#define ARG_DATA_IS_NULL NULL
#define SUCCESS_KANBAN_PROGRAM_CODE 0
#define CONTINUE_DEFAULT_KANBAN_PROGRAM_CODE -1


struct _KanbanApp
{
  GtkApplication parent_instance;
};

static GOptionEntry entries[] =
{
  { "version", 'V', G_OPTION_FLAG_NONE,
    G_OPTION_ARG_NONE, ARG_DATA_IS_NULL,
    "Display program version and exit", NO_ARGUEMENT_PLACEHOLDER },
  { NULL }
};


G_DEFINE_TYPE(KanbanApp, kanban_app, GTK_TYPE_APPLICATION)


static void
kanban_app_init (KanbanApp *app)
{
  g_application_set_option_context_parameter_string (G_APPLICATION (app), 
    "- description of program for help");
  g_application_add_main_option_entries (G_APPLICATION (app), entries);
}

static void
kanban_app_activate (GApplication *app)
{
  KanbanAppWindow *win;

  win = kanban_app_window_new (KANBAN_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
kanban_app_open (GApplication  *app,
                  GFile        **files,
                  gint           n_files,
                  const gchar   *hint)
{
  (void) hint;
  GList *windows;
  KanbanAppWindow *win;
  int i;

  windows = gtk_application_get_windows (GTK_APPLICATION (app));
  if (windows)
    win = KANBAN_APP_WINDOW (windows->data);
  else
    win = kanban_app_window_new (KANBAN_APP (app));

  for (i = 0; i < n_files; i++)
    kanban_app_window_open (win, files[i]);

  gtk_window_present (GTK_WINDOW (win));
}

static gint
kanban_app_handle_local_options (GApplication *app,
                                 GVariantDict *options)
{
  (void) app;
  if (g_variant_dict_contains (options, "version"))
    {
      g_print("Kanban App version information\n");
      return SUCCESS_KANBAN_PROGRAM_CODE;
    }
  else
    {
      return CONTINUE_DEFAULT_KANBAN_PROGRAM_CODE;
    }
  return -1;
}

static void
kanban_app_class_init (KanbanAppClass *klass)
{
  G_APPLICATION_CLASS (klass)->activate = kanban_app_activate;
  G_APPLICATION_CLASS (klass)->open = kanban_app_open;
  G_APPLICATION_CLASS (klass)->handle_local_options = kanban_app_handle_local_options;
}

KanbanApp *
kanban_app_new (void)
{
  return g_object_new (KANBAN_APP_TYPE,
                       "application-id", "com.benwattsjones.kanban",
                       "flags", G_APPLICATION_HANDLES_OPEN,
                       NULL);
}

