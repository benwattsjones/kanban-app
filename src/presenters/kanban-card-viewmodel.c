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

#include "../models/kanban-data.h"

#include <gtk/gtk.h>

struct _KanbanCardViewModel
{
  GObject         parent_instance;

  gint            card_id;
  GtkTextBuffer  *heading;
  GtkTextBuffer  *content;
};

enum
{
  PROP_CARD_ID = 1,
  PROP_HEADING,
  PROP_CONTENT,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

G_DEFINE_TYPE (KanbanCardViewModel, kanban_card_viewmodel, G_TYPE_OBJECT)

static void
kanban_card_viewmodel_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
  KanbanCardViewModel *self = KANBAN_CARD_VIEWMODEL (object);

  switch (property_id)
    {
    case PROP_CARD_ID:
      self->card_id = g_value_get_int (value);
      break;

    case PROP_HEADING:
      gtk_text_buffer_set_text (self->heading, g_value_get_string (value), -1);
      break;

    case PROP_CONTENT:
      gtk_text_buffer_set_text (self->content, g_value_get_string (value), -1);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_card_viewmodel_get_property (GObject    *object,
                                    guint       property_id,
                                    GValue     *value,
                                    GParamSpec *pspec)
{
  KanbanCardViewModel *self = KANBAN_CARD_VIEWMODEL (object);
  GtkTextIter start, end;
  gchar *buffer_text = NULL;

  switch (property_id)
    {
    case PROP_CARD_ID:
      g_value_set_int (value, self->card_id);
      break;

    case PROP_HEADING:
      gtk_text_buffer_get_bounds (self->heading, &start, &end);
      buffer_text = gtk_text_buffer_get_text (self->heading, &start, &end, FALSE);
      g_value_set_string (value, buffer_text);
      g_free (buffer_text);
      break;

    case PROP_CONTENT:
      gtk_text_buffer_get_bounds (self->content, &start, &end);
      buffer_text = gtk_text_buffer_get_text (self->content, &start, &end, FALSE);
      g_value_set_string (value, buffer_text);
      g_free (buffer_text);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kanban_card_viewmodel_finalize (GObject *object)
{
  KanbanCardViewModel *self = KANBAN_CARD_VIEWMODEL (object);
  g_object_unref (self->heading);
  g_object_unref (self->content);
  G_OBJECT_CLASS (kanban_card_viewmodel_parent_class)->finalize (object);
}

static void
kanban_card_viewmodel_init (KanbanCardViewModel *self)
{
  self->content = gtk_text_buffer_new (NULL);
  self->heading = gtk_text_buffer_new (NULL);
}

static void
kanban_card_viewmodel_class_init (KanbanCardViewModelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kanban_card_viewmodel_set_property;
  object_class->get_property = kanban_card_viewmodel_get_property;
  object_class->finalize = kanban_card_viewmodel_finalize;

  obj_properties[PROP_CARD_ID] =
    g_param_spec_int("card-id",
                     "Card ID",
                     "Unique, immutable, kanban card identifier",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_HEADING] =
    g_param_spec_string("heading",
                        "Heading",
                        "Heading of task on a kanban card",
                        NULL,
                        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_properties[PROP_CONTENT] =
    g_param_spec_string("content",
                        "Content",
                        "Details of task on a kanban card",
                        NULL,
                        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

KanbanCardViewModel *
kanban_card_viewmodel_new (const KanbanData *card_data)
{
  const gchar *heading = card_data->heading ? card_data->heading : "";
  const gchar *content = card_data->content ? card_data->content : "";
  return g_object_new (KANBAN_TYPE_CARD_VIEWMODEL,
                       "card-id", card_data->card_id,
                       "heading", heading,
                       "content", content,
                       NULL);
}

void
kanban_card_viewmodel_update_contents (KanbanCardViewModel *self,
                                       const gchar         *heading,
                                       const gchar         *content)
{
  if (heading)
    {
      gtk_text_buffer_set_text (self->heading, heading, -1);
      g_object_notify_by_pspec (G_OBJECT (self), obj_properties[PROP_HEADING]);
    }
  if (content)
    {
      gtk_text_buffer_set_text (self->content, content, -1);
      g_object_notify_by_pspec (G_OBJECT (self), obj_properties[PROP_CONTENT]);
    }
}

GtkTextBuffer *
kanban_card_viewmodel_get_content (KanbanCardViewModel *self)
{
  return self->content;
}

GtkTextBuffer *
kanban_card_viewmodel_get_heading (KanbanCardViewModel *self)
{
  return self->heading;
}

