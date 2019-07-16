/* src/models/model-presenter-interface.h
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

#ifndef MODEL_PRESENTER_INTERFACE_H
#define MODEL_PRESENTER_INTERFACE_H

#include "kanban-cards.h"

typedef void (*KanbanChangeNotification) (void             *instance,
                                          const KanbanCard *card_data);

typedef struct
{
  void                     *instance;
  KanbanChangeNotification  notification;
} KanbanModelObserver;

/* PUBLIC: INTERFACE FOR PRESENTER USE */
void attach_observer                (const KanbanModelObserver *observer);
void detach_observer                (const KanbanModelObserver *observer);

/* PRIVATE: FOR INTERNAL MODEL USE ONLY */
void emit_kanban_card_change_signal (const KanbanCard          *card_data);

#endif /* MODEL_PRESENTER_INTERFACE_H */

