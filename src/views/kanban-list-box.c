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
#include "../presenters/kanban-card-viewmodel.h"

#include <gtk/gtk.h>
#include <gio/gio.h>

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

GtkWidget *create_card_widget_func (gpointer item, gpointer user_data);

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
      self->card_data = g_value_get_pointer (value);
      g_print("Setting card data\n");
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
kanban_list_box_constructed (GObject *object)
{
  KanbanListBox *self = KANBAN_LIST_BOX (object);
  gtk_list_box_bind_model (GTK_LIST_BOX (self), //&self->parent_instance,
                           G_LIST_MODEL (self->card_data),
                           create_card_widget_func,
                           NULL, g_free);
  g_print("init list box\n");
  g_print("list box model addr: %p\n", (void *) self->card_data);
  g_print("init gui list len: %d\n", g_list_model_get_n_items (G_LIST_MODEL (self->card_data)));
}

static void
kanban_list_box_init (KanbanListBox *self)
{
  // _constructed() used to bind model as _init() called before properties set
}

static void
kanban_list_box_class_init (KanbanListBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_list_box_set_property;
  object_class->get_property = kanban_list_box_get_property;
  object_class->constructed = kanban_list_box_constructed;

  obj_properties[PROP_CARD_DATA] =
    g_param_spec_pointer("card-data",
                        "Card Data",
                        "KanbanListStore Card data to bind to via GListModel iface",
                       // KANBAN_LIST_STORE_TYPE,
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

GtkWidget *
create_card_widget_func (gpointer item,
                         gpointer user_data)
{
  (void) user_data; // NULL passed
  KanbanCardViewModel *new_card_viewmodel;
  gchar *card_heading = NULL;
  gchar *card_content = NULL;

  new_card_viewmodel = KANBAN_CARD_VIEWMODEL (item);
  g_object_get (new_card_viewmodel,
                "heading", &card_heading,
                "content", &card_content,
                NULL);
  g_print("GUI INFO: H: %s, C: %s\n", card_heading, card_content);

  // TODO: bind notify signal of card to update widgets
  GtkWidget *grid = gtk_grid_new();
  GtkWidget *heading_label = gtk_label_new (card_heading);
  GtkWidget *content_label = gtk_label_new (card_content);

  gtk_grid_attach (GTK_GRID (grid), heading_label, 0, 0, 1, 1);
  gtk_grid_attach (GTK_GRID (grid), content_label, 0, 1, 1, 1);
  gtk_widget_show_all (grid);

  return grid;
}

