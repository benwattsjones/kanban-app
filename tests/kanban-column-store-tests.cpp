/* tests/kanban-column-store-tests.cpp
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
  #include "../src/presenters/kanban-column-store.h"
  #include "../src/presenters/kanban-list-store.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"


// Stubs:
extern "C"
{
  KanbanListStore *initialize_viewmodel (gint col_id)
  {
    (void) col_id;
    return nullptr;
  }

  void destroy_viewmodel (KanbanListStore *viewmodel)
  {
    (void) viewmodel;
  }
}

// Test Fixtures:
class KanbanColumnStoreTests : public ::testing::Test
{
protected:
  KanbanColumnStore *viewmodel;

  void SetUp() override
  {
    viewmodel = kanban_column_store_new ();
  }

  void TearDown() override
  {
    kanban_column_store_destroy (viewmodel);
  }
};

// Tests:
TEST_F(KanbanColumnStoreTests, checkKanbanColumnStoreCreated)
{
  ASSERT_NE (viewmodel, nullptr);
}


