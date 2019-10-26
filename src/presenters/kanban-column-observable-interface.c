/* src/presenters/kanban-column-observable-interface.c
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

#include "kanban-column-observable-interface.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

G_DEFINE_INTERFACE (KanbanColumnObservable, kanban_column_observable, G_TYPE_LIST_MODEL)

static void
kanban_column_observable_default_init (KanbanColumnObservableInterface *iface)
{
  // Add properties and signals to interface here
  (void) iface;
}

gint
kanban_column_observable_get_id (KanbanColumnObservable *self)
{
#ifdef TESTING_ONLY_ACCESS
  if (!self)
    return 0;
#endif
  KanbanColumnObservableInterface *iface;
  g_return_val_if_fail (KANBAN_IS_COLUMN_OBSERVABLE (self), 0);
  iface = KANBAN_COLUMN_OBSERVABLE_GET_IFACE (self);
  g_return_val_if_fail (iface->get_id != NULL, 0);
  return iface->get_id (self);

}

GtkTextBuffer *
kanban_column_observable_get_heading (KanbanColumnObservable *self)
{
#ifdef TESTING_ONLY_ACCESS
  if (!self)
    return NULL;
#endif
  KanbanColumnObservableInterface *iface;
  g_return_val_if_fail (KANBAN_IS_COLUMN_OBSERVABLE (self), NULL);
  iface = KANBAN_COLUMN_OBSERVABLE_GET_IFACE (self);
  g_return_val_if_fail (iface->get_heading != NULL, NULL);
  return iface->get_heading (self);
}

