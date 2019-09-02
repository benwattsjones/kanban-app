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

#include "model-observer-interface.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

void register_kanban_viewmodel_observer   (ModelObserverInterface *observer);
void deregister_kanban_viewmodel_observer (ModelObserverInterface *observer);

G_END_DECLS

#endif /* MODEL_OBSERVER_H */

