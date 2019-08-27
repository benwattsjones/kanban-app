/* tests/kanban-list-store-tests.cpp
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
  #include "../src/presenters/kanban-list-store.h"
  #include "../src/models/kanban-cards.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  void register_kanban_viewmodel_observer   (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }

  void deregister_kanban_viewmodel_observer (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }
}

// Test Fixtures:
class KanbanListStoreTests : public ::testing::Test
{
protected:
  KanbanCard card_data;
  KanbanListStore *viewmodel;

  void SetUp() override
  {
    card_data.card_id = 1;
    card_data.column_id = 2;
    card_data.heading = NULL;
    card_data.content = NULL;
    card_data.priority = 3;

    viewmodel = initialize_viewmodel ();
  }

  void TearDown() override
  {
    destroy_viewmodel (viewmodel);
  }
};

// Tests:
TEST_F(KanbanListStoreTests, checkKanbanListStoreCreated)
{
  ASSERT_NE(viewmodel, nullptr);
}


