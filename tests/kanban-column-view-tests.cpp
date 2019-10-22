/* tests/kanban-column-view-tests.cpp
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
  #include "../src/views/kanban-column-view.h"
  #include "../src/presenters/kanban-column-observable-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
}

// Test Fixtures:
class KanbanColumnViewTests : public ::testing::Test
{
protected:
  KanbanColumnView *column;

  void SetUp() override
  {
    gtk_init (0, NULL);
    column = kanban_column_view_new (NULL);
  }

  void TearDown() override
  {
    g_object_ref_sink (column);
  }
};


// Tests:
TEST_F (KanbanColumnViewTests,
        New_GtkInitializedNewCalled_ColumnCreatedNotNull)
{
  ASSERT_NE (column, nullptr);
}

