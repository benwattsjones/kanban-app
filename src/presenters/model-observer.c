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

#include "kanban-tree-store.h"
#include "../models/model-presenter-interface.h"
#include "../models/kanban-cards.h"

#include <assert.h>
#include <stddef.h>


static void 
kanban_model_changed (void             *instance, 
                      const KanbanCard *card_data)
{
  KanbanTreeStore *viewmodel = instance;
  assert (viewmodel != NULL);
  if (card_data->card_id == 0)
    {
      // change is to a column. TODO: implement change column funciton.
    }
  else
    {
      viewmodel_change_card (card_data);
    }
}

void 
register_kanban_viewmodel_observer (KanbanTreeStore *viewmodel)
{
  assert (viewmodel != NULL);
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  attach_observer (&observer);
  test_observers ();
}

void
deregister_kanban_viewmodel_observer (KanbanTreeStore *viewmodel)
{
  KanbanModelObserver observer = { .instance = viewmodel,
                                   .notification = kanban_model_changed };
  detach_observer (&observer);
}

