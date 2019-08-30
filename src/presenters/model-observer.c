/* src/presenters/model-observer.c
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

#include "model-observer.h"

#include "kanban-column-store.h"
#include "../models/model-presenter-interface.h"
#include "../models/kanban-cards.h"

#include <assert.h>
#include <stddef.h>


static void 
kanban_model_changed (void             *instance, 
                      const KanbanCard *card_data)
{
  KanbanColumnStore *viewmodel = instance;
  assert (viewmodel != NULL);
  
  if (card_data->card_id == 0)
    kanban_column_store_edit_column (viewmodel, card_data);
  else if (card_data->column_id == 0)
    kanban_column_store_edit_card (viewmodel, card_data);
  else if (card_data->heading == NULL)
    kanban_column_store_move_card (viewmodel, card_data);
  else
    kanban_column_store_add_card (viewmodel, card_data);
}

void 
register_kanban_viewmodel_observer (KanbanColumnStore *viewmodel)
{
  assert (viewmodel != NULL);
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  attach_observer (&observer);
  test_observers ();
}

void
deregister_kanban_viewmodel_observer (KanbanColumnStore *viewmodel)
{
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  detach_observer (&observer);
}

