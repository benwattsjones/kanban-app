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

#include "model-observer-interface.h"
#include "../models/model-presenter-interface.h"
#include "../models/kanban-data.h"

#include <assert.h>
#include <stddef.h>


static void 
kanban_model_changed (void             *instance, 
                      const KanbanData *card_data)
{
  ModelObserverInterface *observer = instance;
  assert (observer != NULL);
  
  if (card_data->card_id == 0)
    observer->edit_column (observer->viewmodel, card_data);
  else if (card_data->column_id == 0)
    observer->edit_card (observer->viewmodel, card_data);
  else if (card_data->heading == NULL)
    observer->move_card (observer->viewmodel, card_data);
  else
    observer->add_card (observer->viewmodel, card_data);
}

void 
register_kanban_viewmodel_observer (ModelObserverInterface *observer)
{
  assert (observer != NULL);
  KanbanModelObserver observer_wrap = { .instance = observer,
                                        .notification = kanban_model_changed };
  attach_observer (&observer_wrap);
  test_observers ();
}

void
deregister_kanban_viewmodel_observer (ModelObserverInterface *observer)
{
  KanbanModelObserver observer_wrap = { .instance = observer,
                                        .notification = kanban_model_changed };
  detach_observer (&observer_wrap);
}

