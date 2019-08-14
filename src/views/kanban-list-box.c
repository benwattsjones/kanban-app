/* src/views/kanban-list-box.c
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

#include "kanban-list-box.h"

#include "../presenters/kanban-list-store.h"

#include <gtk/gtk.h>

struct _KanbanListBox
{
  GtkListBox       parent_instance;

  KanbanListStore *card_data;
};

enum
{
  PROP_CARD_DATA = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanListBox, kanban_list_box, GTK_TYPE_LIST_BOX)

static void
kanban_list_box_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  KanbanListBox *self = KANBAN_LIST_BOX (object);

  switch (property_id)
    {
    case PROP_CARD_DATA:
      self->card_data = g_value_get_object (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_list_box_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  KanbanListBox *self = KANBAN_LIST_BOX (object);

  switch (property_id)
    {
    case PROP_CARD_DATA:
      g_value_set_object (value, self->card_data);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}



static void
kanban_list_box_init (KanbanListBox *self)
{

}

static void
kanban_list_box_class_init (KanbanListBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_list_box_set_property;
  object_class->get_property = kanban_list_box_get_property;

  obj_properties[PROP_CARD_DATA] =
    g_param_spec_object("card-data",
                        "Card Data",
                        "KanbanListStore Card data to bind to via GListModel iface",
                        KANBAN_LIST_STORE_TYPE,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

KanbanListBox *
kanban_list_box_new (KanbanListStore *card_data)
{
  return g_object_new (KANBAN_LIST_BOX_TYPE,
                       "card-data", card_data,
                       NULL);
}

