/* src/presenters/kanban-card-viewmodel.c
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

#include "kanban-card-viewmodel.h"

#include <gtk/gtk.h>

struct _KanbanCardViewModel
{
  GObject  parent_instance;
  gint     card_id;
  gint     column_id; /* may wish to replace with a column object */
  gchar   *heading;
  gchar   *content;
  gint     priority;
};

enum
{
  PROP_CARD_ID = 1,
  PROP_COLUMN_ID,
  PROP_HEADING,
  PROP_CONTENT,
  PROP_PRIORITY,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanCardViewModel, kanban_card_viewmodel, G_TYPE_OBJECT)

static void
kanban_card_viewmodel_init (KanbanCardViewModel *self)
{

}

static void
kanban_card_viewmodel_class_init (KanbanCardViewModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  obj_properties[PROP_CARD_ID] =
    g_param_spec_int("card-id",
                     "Card ID",
                     "Unique, immutable, kanban card identifier",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT_ONLY);
  obj_properties[PROP_COLUMN_ID] =
    g_param_spec_int("column-id",
                     "Column ID", 
                     "Unique, immutable kanban column identifier",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_properties[PROP_HEADING] =
    g_param_spec_string("heading",
                        "Heading",
                        "Heading of task on a kanban card",
                        NULL,
                        G_PARAM_READWRITE);
  obj_properties[PROP_CONTENT] =
    g_param_spec_string("content",
                        "Content",
                        "Details of task on a kanban card",
                        NULL,
                        G_PARAM_READWRITE);
  obj_properties[PROP_PRIORITY] =
    g_param_spec_int("priority",
                     "Priority",
                     "Priority (i.e. position) of kanban card in its column",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);

}

KanbanCardViewModel *
kanban_card_viewmodel_new (gint card_id,
                           gint column_id,
                           gint priority)
{
  return g_object_new (KANBAN_CARD_VIEWMODEL_TYPE,
                       "card-id", card_id,
                       "column-id", column_id,
                       "priority", priority,
                       NULL);
}
