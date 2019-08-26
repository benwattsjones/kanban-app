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
    card_data.column_id = 1;
    card_data.heading = NULL;
    card_data.content = NULL;
    card_data.priority = 1;
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

