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

#include "../models/kanban-cards.h"

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

    case PROP_COLUMN_ID:
      self->column_id = g_value_get_int (value);
      break;

    case PROP_HEADING:
      g_free (self->heading);
      self->heading = g_value_dup_string (value);
      break;

    case PROP_CONTENT:
      g_free (self->content);
      self->content = g_value_dup_string (value);
      break;

    case PROP_PRIORITY:
      self->priority = g_value_get_int (value);
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

  switch (property_id)
    {
    case PROP_CARD_ID:
      g_value_set_int (value, self->card_id);
      break;

    case PROP_COLUMN_ID:
      g_value_set_int (value, self->column_id);
      break;

    case PROP_HEADING:
      g_value_set_string (value, self->heading);
      break;

    case PROP_CONTENT:
      g_value_set_string (value, self->content);
      break;

    case PROP_PRIORITY:
      g_value_set_int (value, self->priority);
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
  g_free(self->heading);
  g_free(self->content);
  G_OBJECT_CLASS (kanban_card_viewmodel_parent_class)->finalize (object);
}

static void
kanban_card_viewmodel_init (KanbanCardViewModel *self)
{
  (void) self;
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
                        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_properties[PROP_CONTENT] =
    g_param_spec_string("content",
                        "Content",
                        "Details of task on a kanban card",
                        NULL,
                        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  obj_properties[PROP_PRIORITY] =
    g_param_spec_int("priority",
                     "Priority",
                     "Priority (i.e. position) of kanban card in its column",
                     0, G_MAXINT, 0,
                     G_PARAM_CONSTRUCT | G_PARAM_READWRITE);
  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);

}

KanbanCardViewModel *
kanban_card_viewmodel_new (const KanbanCard *card_data)
{
  return g_object_new (KANBAN_CARD_VIEWMODEL_TYPE,
                       "card-id", card_data->card_id,
                       "column-id", card_data->column_id,
                       "heading", card_data->heading,
                       "content", card_data->content,
                       "priority", card_data->priority,
                       NULL);
}

void
kanban_card_viewmodel_update_contents (KanbanCardViewModel *self,
                                       const gchar         *heading,
                                       const gchar         *content)
{
  if (heading)
    {
      g_free (self->heading);
      self->heading = g_strdup (heading);
      g_object_notify_by_pspec (G_OBJECT (self), obj_properties[PROP_HEADING]);
    }
  if (content)
    {
      g_free (self->content);
      self->content = g_strdup (content);
      g_object_notify_by_pspec (G_OBJECT (self), obj_properties[PROP_CONTENT]);
    }
}

