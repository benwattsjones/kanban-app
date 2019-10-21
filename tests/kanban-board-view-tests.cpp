/* tests/kanban-board-view-tests.cpp
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
  #include "../src/views/kanban-board-view.h"
  #include "../src/views/kanban-list-box.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  KanbanListBox *kanban_list_box_new (KanbanColumnObservable *column_data)
  {
    (void) column_data;
    return NULL;
  }
}

// Test Fixtures:
class KanbanBoardViewTests : public ::testing::Test
{
protected:
  KanbanBoardView *board;

  void SetUp() override
  {
    gtk_init (0, NULL);
    board = kanban_board_view_new ();
  }

  void TearDown() override
  {
    g_object_ref_sink (board);
  }
};


// Tests:
TEST_F (KanbanBoardViewTests,
        New_GtkInitializedNewCalled_BoardCreatedNotNull)
{
  ASSERT_NE (board, nullptr);
}

