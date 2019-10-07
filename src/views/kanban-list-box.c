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

#include "../presenters/kanban-card-viewmodel.h"
#include <kanban-config.h>

#include <gtk/gtk.h>
#include <gio/gio.h>

struct _KanbanListBox
{
  GtkListBox   parent_instance;

  GListModel  *column_data;
};

enum
{
  PROP_COLUMN_DATA = 1,
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
    case PROP_COLUMN_DATA:
      self->column_data = g_value_get_pointer (value);
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
    case PROP_COLUMN_DATA:
      g_value_set_pointer (value, self->column_data);
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
  gtk_list_box_bind_model (GTK_LIST_BOX (self), self->column_data,
                           create_card_widget_func, NULL, g_free);
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

  obj_properties[PROP_COLUMN_DATA] =
    g_param_spec_pointer("column-data",
                         "Column Data",
                         "Card data of single column to bind to via GListModel iface",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

KanbanListBox *
kanban_list_box_new (GListModel *column_data)
{
  return g_object_new (KANBAN_LIST_BOX_TYPE,
                       "column-data", column_data,
                       NULL);
}

GtkWidget *
create_card_widget_func (gpointer item,
                         gpointer user_data)
{
  (void) user_data; // NULL passed
  KanbanCardViewModel *viewmodel = KANBAN_CARD_VIEWMODEL (item);

  gchar *card_heading = NULL;
  g_object_get (viewmodel,
                "heading", &card_heading,
                NULL);
  GtkTextBuffer *card_content = kanban_card_viewmodel_get_content (viewmodel);

  GtkBuilder *builder = gtk_builder_new_from_resource (GRESOURCE_PREFIX "card.ui");
  GObject *heading_widget = gtk_builder_get_object (builder, "heading-widget");
  GObject *content_widget = gtk_builder_get_object (builder, "content-widget");
  GObject *card_widget = gtk_builder_get_object (builder, "card-widget");

  // TODO: bind notify signal of card to update widgets
  gtk_label_set_text (GTK_LABEL (heading_widget), card_heading);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (content_widget), card_content);

  return GTK_WIDGET (card_widget);
}

