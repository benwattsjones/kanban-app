/* src/command-line-handler.c
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

#define NO_ARGUEMENT_PLACEHOLDER NULL
#define NO_INTERNATIONAL_GET_TEXT NULL
#define OPEN_DEFAULT_DISPLAY TRUE

static gboolean version_option = FALSE;

static GOptionEntry entries[] =
{
  { "version", 'V', G_OPTION_FLAG_NONE,
    G_OPTION_ARG_NONE, &version_option,
    "Display program version and exit", NO_ARGUEMENT_PLACEHOLDER },
  { NULL }
};

void
set_recognised_cmd_options (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *context;

  context = g_option_context_new ("- description of program for help");
  g_option_context_add_main_entries (context, entries, NO_INTERNATIONAL_GET_TEXT);
  g_option_context_add_group (context, gtk_get_option_group (OPEN_DEFAULT_DISPLAY));
  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_print ("Failed to pass command options: %s\n", error->message);
      exit(1);
    }
}

