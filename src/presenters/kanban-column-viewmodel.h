/* src/presenters/kanban-column-viewmodel.h
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

#ifndef KANBAN_COLUMN_VIEWMODEL_H
#define KANBAN_COLUMN_VIEWMODEL_H

#include "../models/kanban-data.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_TYPE_COLUMN_VIEWMODEL (kanban_column_viewmodel_get_type ())

G_DECLARE_FINAL_TYPE (KanbanColumnViewModel, kanban_column_viewmodel, KANBAN, COLUMN_VIEWMODEL, GObject)

KanbanColumnViewModel  *kanban_column_viewmodel_new
                                              (gint                    col_id,
                                               const gchar            *col_name);

void                    kanban_column_viewmodel_destroy 
                                              (gpointer                vself);

void                    kanban_column_viewmodel_set_heading 
                                              (KanbanColumnViewModel  *self,
                                               const gchar            *text);

GSequenceIter          *kanban_column_viewmodel_new_card
                                              (KanbanColumnViewModel  *self,
                                               const KanbanData       *card_data);

GSequence              *kanban_column_viewmodel_get_sequence
                                              (KanbanColumnViewModel  *self);

void                    kanban_column_viewmodel_move_card 
                                              (KanbanColumnViewModel  *current_column,
                                               KanbanColumnViewModel  *new_column,
                                               GSequenceIter          *card_iter,
                                               gint                    new_position);

G_END_DECLS

#endif /* KANBAN_COLUMN_VIEWMODEL_H */

