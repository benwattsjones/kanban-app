/* tests/kanban-column-observable-interface-tests.cpp
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
  #include "../src/presenters/kanban-column-observable-interface.h"
  #include "../src/presenters/kanban-column-viewmodel.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

#define COLUMN_ID 1

extern "C"
{
}

// Test Fixtures:
class KanbanColumnObservableInterfaceTests : public ::testing::Test
{
protected:
  KanbanColumnViewModel *viewmodel;

  void SetUp() override
  {
    viewmodel = kanban_column_viewmodel_new (COLUMN_ID, "col heading");
  }

  void TearDown() override
  {
    kanban_column_viewmodel_destroy (viewmodel);
  }
};

// Tests:
TEST_F (KanbanColumnObservableInterfaceTests,
        New_ValidKanbanColumnViewModelPassed_InterfaceAvailable)
{
  SUCCEED ();
}

