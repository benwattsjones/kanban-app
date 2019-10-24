/* src/presenters/kanban-column-observable-interface.h
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

#ifndef KANBAN_COLUMN_OBSERVABLE_INTERFACE_H
#define KANBAN_COLUMN_OBSERVABLE_INTERFACE_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_COLUMN_OBSERVABLE (kanban_column_observable_get_type ())

G_DECLARE_INTERFACE (KanbanColumnObservable, kanban_column_observable,
                     KANBAN, COLUMN_OBSERVABLE, GObject)

struct _KanbanColumnObservableInterface
{
  GTypeInterface parent_iface;

  GtkTextBuffer *   (*get_heading)    (KanbanColumnObservable *self);
};

GtkTextBuffer  *kanban_column_observable_get_heading  (KanbanColumnObservable *self);

G_END_DECLS

#endif /* KANBAN_COLUMN_OBSERVABLE_INTERFACE_H */

