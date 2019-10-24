/* src/views/kanban-column-view.h
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

#ifndef KANBAN_COLUMN_VIEW_H
#define KANBAN_COLUMN_VIEW_H

#include "../presenters/kanban-column-observable-interface.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_COLUMN_VIEW (kanban_column_view_get_type ())

G_DECLARE_FINAL_TYPE (KanbanColumnView, kanban_column_view, KANBAN, COLUMN_VIEW, GtkBox)

KanbanColumnView *kanban_column_view_new          (KanbanColumnObservable *column_data);

#ifdef TESTING_ONLY_ACCESS

GtkWidget        *kanban_column_view_get_contents      (KanbanColumnView  *self);

gchar            *kanban_column_view_get_card_heading  (KanbanColumnView  *self,
                                                        gint               priority);

#endif /* TESTING_ONLY_ACCESS */

G_END_DECLS

#endif /* KANBAN_COLUMN_VIEW_H */

