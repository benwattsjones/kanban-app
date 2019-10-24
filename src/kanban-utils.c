/* src/kanban-utils.c
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

#include "kanban-utils.h"

#ifdef TESTING_ONLY_ACCESS

GtkWidget *
kanban_utils_find_widget_by_name (GtkWidget   *parent,
                                  const gchar *name)
{
  GtkWidget *named_widget = NULL;
  GList *children, *l;
  if (g_strcmp0 (gtk_buildable_get_name (GTK_BUILDABLE (parent)), name) == 0)
    {
      named_widget = parent;
    }
  else if (GTK_IS_BIN (parent))
    {
      named_widget = kanban_utils_find_widget_by_name (
                         gtk_bin_get_child (GTK_BIN (parent)), name);
    }
  else if (GTK_IS_CONTAINER (parent))
    {
      children = gtk_container_get_children (GTK_CONTAINER (parent));
      for (l = children; l != NULL && named_widget == NULL; l = l->next)
        {
          named_widget = kanban_utils_find_widget_by_name (l->data, name);
        }
      g_list_free (children);
    }
  return named_widget;
}

gchar *
kanban_utils_get_text_from_view (GtkTextView *textview)
{
  GtkTextIter start, end;
  GtkTextBuffer *text_buffer;

  text_buffer = gtk_text_view_get_buffer (textview);
  gtk_text_buffer_get_bounds (text_buffer, &start, &end);
  return gtk_text_buffer_get_text (text_buffer, &start, &end, FALSE);

}

#endif /* TESTING_ONLY_ACCESS */

