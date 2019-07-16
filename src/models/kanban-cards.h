/* src/models/kanban-cards.h
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

#ifndef KANBAN_CARDS_H
#define KANBAN_CARDS_H

typedef struct
{
  int card_id;
  int column_id;
  char *heading;
  char *content;
  int priority;
} KanbanCard;

void test_observers ();

#endif /* KANBAN_CARDS_H */

