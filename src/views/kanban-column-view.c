/* src/views/kanban-column-view.c
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

#include "kanban-column-view.h"

#include "../presenters/kanban-card-viewmodel.h"
#include "../presenters/kanban-column-observable-interface.h"
#include <kanban-config.h>

#include <gtk/gtk.h>
#include <gio/gio.h>

struct _KanbanColumnView
{
  GtkBox                   parent_instance;

  GtkWidget               *column_heading;
  GtkWidget               *column_contents;

  KanbanColumnObservable  *column_data;
};

enum
{
  PROP_COLUMN_DATA = 1,
  N_PROPERTIES
};

GtkWidget *create_card_widget_func (gpointer item, gpointer user_data);

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanColumnView, kanban_column_view, GTK_TYPE_BOX)

static void
kanban_column_view_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  KanbanColumnView *self = KANBAN_COLUMN_VIEW (object);

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
kanban_column_view_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  KanbanColumnView *self = KANBAN_COLUMN_VIEW (object);

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
kanban_column_view_constructed (GObject *object)
{
  KanbanColumnView *self = KANBAN_COLUMN_VIEW (object);

  gtk_list_box_bind_model (GTK_LIST_BOX (self->column_contents),
                           G_LIST_MODEL (self->column_data),
                           create_card_widget_func, NULL, g_free);

  gtk_text_view_set_buffer (GTK_TEXT_VIEW (self->column_heading),
                            kanban_column_observable_get_heading (self->column_data));
}

static void
kanban_column_view_init (KanbanColumnView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
kanban_column_view_class_init (KanbanColumnViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_column_view_set_property;
  object_class->get_property = kanban_column_view_get_property;
  object_class->constructed = kanban_column_view_constructed;

  obj_properties[PROP_COLUMN_DATA] =
    g_param_spec_pointer("column-data",
                         "Column Data",
                         "Column data to bind to via KanbanColumnObservable iface",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               GRESOURCE_PREFIX "column.ui");
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        KanbanColumnView, column_heading);
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        KanbanColumnView, column_contents);
}

KanbanColumnView *
kanban_column_view_new (KanbanColumnObservable *column_data)
{
  return g_object_new (KANBAN_TYPE_COLUMN_VIEW,
                       "column-data", column_data,
                       NULL);
}

GtkWidget *
create_card_widget_func (gpointer item,
                         gpointer user_data)
{
  (void) user_data; // NULL passed
  KanbanCardViewModel *viewmodel = KANBAN_CARD_VIEWMODEL (item);

  GtkTextBuffer *card_heading = kanban_card_viewmodel_get_heading (viewmodel);
  GtkTextBuffer *card_content = kanban_card_viewmodel_get_content (viewmodel);

  GtkBuilder *builder = gtk_builder_new_from_resource (GRESOURCE_PREFIX "card.ui");
  GObject *heading_widget = gtk_builder_get_object (builder, "heading-widget");
  GObject *content_widget = gtk_builder_get_object (builder, "content-widget");
  GObject *card_widget = gtk_builder_get_object (builder, "card-widget");

  gtk_text_view_set_buffer (GTK_TEXT_VIEW (heading_widget), card_heading);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (content_widget), card_content);

  return GTK_WIDGET (card_widget);
}

