/* src/presenters/model-observer.h
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

#ifndef MODEL_OBSERVER_H
#define MODEL_OBSERVER_H

typedef struct KanbanViewModel *KanbanViewModelPtr;

KanbanViewModelPtr create_kanban_viewmodel  ();
void               destroy_kanban_viewmodel (KanbanViewModelPtr viewmodel);

#endif /* MODEL_OBSERVER_H */

