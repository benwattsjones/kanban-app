/* src/presenters/kanban-card-viewmodel.h
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

#ifndef KANBAN_CARD_VIEWMODEL_H
#define KANBAN_CARD_VIEWMODEL_H

#include "../models/kanban-cards.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KANBAN_CARD_VIEWMODEL_TYPE (kanban_card_viewmodel_get_type ())

G_DECLARE_FINAL_TYPE (KanbanCardViewModel, kanban_card_viewmodel, KANBAN, CARD_VIEWMODEL, GObject)

KanbanCardViewModel *kanban_card_viewmodel_new (const KanbanCard    *card_data);

void                 kanban_card_viewmodel_update_contents 
                                               (KanbanCardViewModel *self,
                                                gchar               *heading,
                                                gchar               *content);

G_END_DECLS

#endif /* KANBAN_CARD_VIEWMODEL_H */
