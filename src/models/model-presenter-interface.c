/* src/models/model-presenter-interface.c
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

#include <stdlib.h>
#include <assert.h>

#include "kanban-cards.h"

#include "model-presenter-interface.h"

/* Linked list implementation to store observers list. NOTE:
 *   - The root struct (observers_linked_list) never holds data.
 *     This makes memory management and list access easy.
 *   - The append and remove functions do not check for duplicates -
 *     appending duplicates will cause memory leaks and unexpected behaviour.
 *   - NOT thread safe - race condition if observers added or removed during
 *     call to change_kanban().
 * These are not big concerns as there is only intended to be one observer
 * added, and for it to be removed only on program termination.
 */
typedef struct _ObserversListNode
{
  KanbanModelObserver observer_item;
  struct _ObserversListNode *next;
} ObserversListNode;

static ObserversListNode observers_linked_list;

static void
append_observer_to_list(const KanbanModelObserver *observer)
{
  ObserversListNode *iter = &observers_linked_list;
  while (iter->next != NULL)
    iter = iter->next;
  ObserversListNode *node = (ObserversListNode *) malloc (sizeof (ObserversListNode));
  assert (node != NULL);
  node->observer_item.instance = observer->instance;
  node->observer_item.notification = observer->notification;
  node->next = NULL;
  iter->next = node;
}

static void
remove_observer_from_list(const KanbanModelObserver *observer)
{
  ObserversListNode *iter = &observers_linked_list;
  ObserversListNode *remove_next_node = NULL;
  ObserversListNode *node_to_remove = NULL;
  while (iter->next != NULL)
    {
      if (iter->next->observer_item.instance == observer->instance &&
          iter->next->observer_item.notification == observer->notification)
        {
          remove_next_node = iter;
          node_to_remove = iter->next;
        }
      iter = iter->next;
    }
  if (!node_to_remove)
    return;
  remove_next_node->next = node_to_remove->next;
  free(node_to_remove);
  node_to_remove = NULL;
}

void 
attach_observer(const KanbanModelObserver *observer)
{
  assert (observer != NULL);
  append_observer_to_list (observer);
}

void 
detach_observer(const KanbanModelObserver *observer)
{
  assert (observer != NULL);
  remove_observer_from_list (observer);
}

void 
emit_kanban_card_change_signal(const KanbanCard *card_data)
{
  ObserversListNode *observers_iter = observers_linked_list.next;
  KanbanModelObserver *active_observer;
  while (observers_iter != NULL)
    {
      active_observer = &observers_iter->observer_item;
      active_observer->notification (active_observer->instance, card_data);
      observers_iter = observers_iter->next;
    }
}

