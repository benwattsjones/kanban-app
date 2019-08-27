/* tests/kanban-card-viewmodel-tests.cpp
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

extern "C"
{
  #include "../src/presenters/kanban-card-viewmodel.h"
  #include "../src/models/kanban-cards.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Test Fixtures:
class KanbanCardViewModelTests : public ::testing::Test
{
protected:
  KanbanCard card_data;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = NULL;
    card_data.content = NULL;
    card_data.priority = 3;
  }

  void TearDown() override
  {
  }
};

// Tests:
TEST_F(KanbanCardViewModelTests, checkCardIdPropertyStored)
{
  KanbanCardViewModel *viewmodel = kanban_card_viewmodel_new (&card_data);
  int stored_property;
  g_object_get (viewmodel, "card-id", &stored_property, NULL);
  EXPECT_EQ(stored_property, card_data.card_id);
  g_object_unref (viewmodel);
}

TEST_F(KanbanCardViewModelTests, checkColumnIdPropertyStored)
{
  KanbanCardViewModel *viewmodel = kanban_card_viewmodel_new (&card_data);
  int stored_property;
  g_object_get (viewmodel, "column-id", &stored_property, NULL);
  EXPECT_EQ(stored_property, card_data.column_id);
  g_object_unref (viewmodel);
}

TEST_F(KanbanCardViewModelTests, checkPriorityPropertyStored)
{
  KanbanCardViewModel *viewmodel = kanban_card_viewmodel_new (&card_data);
  int stored_property;
  g_object_get (viewmodel, "priority", &stored_property, NULL);
  EXPECT_EQ(stored_property, card_data.priority);
  g_object_unref (viewmodel);
}

TEST_F(KanbanCardViewModelTests, checkCardHeadingPropertyStored)
{
  card_data.heading = g_strdup("card heading");
  KanbanCardViewModel *viewmodel = kanban_card_viewmodel_new (&card_data);
  char *stored_property;
  g_object_get (viewmodel, "heading", &stored_property, NULL);
  EXPECT_STREQ(stored_property, card_data.heading);
  g_object_unref (viewmodel);
  free (card_data.heading);
  free (stored_property);
}

TEST_F(KanbanCardViewModelTests, checkCardContentPropertyStored)
{
  card_data.content = g_strdup("card content");
  KanbanCardViewModel *viewmodel = kanban_card_viewmodel_new (&card_data);
  char *stored_property;
  g_object_get (viewmodel, "content", &stored_property, NULL);
  EXPECT_STREQ(stored_property, card_data.content);
  g_object_unref (viewmodel);
  free (card_data.content);
  free (stored_property);
}

