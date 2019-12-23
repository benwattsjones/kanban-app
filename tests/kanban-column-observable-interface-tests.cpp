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
  #include "presenters/kanban-column-observable-interface.h"
  #include "presenters/kanban-column-viewmodel.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

#define COLUMN_ID 1
#define COLUMN_HEADING "col heading"

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
    viewmodel = kanban_column_viewmodel_new (COLUMN_ID, COLUMN_HEADING);
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
  KanbanColumnObservable *column_iface;
  GListModel *list_iface;
  column_iface = KANBAN_COLUMN_OBSERVABLE (viewmodel);
  list_iface = G_LIST_MODEL (column_iface);
  EXPECT_TRUE (KANBAN_IS_COLUMN_OBSERVABLE (column_iface));
  EXPECT_TRUE (G_IS_LIST_MODEL (list_iface));
}

TEST_F (KanbanColumnObservableInterfaceTests,
        GetHeading_ValidKanbanColumnViewModelPassed_PointerToHeadingReturned)
{
  KanbanColumnObservable *column_iface;
  GtkTextIter start, end;
  gchar *buffer_text;
  GtkTextBuffer *heading;
  column_iface = KANBAN_COLUMN_OBSERVABLE (viewmodel);

  heading = kanban_column_observable_get_heading (column_iface);
  gtk_text_buffer_get_bounds (heading, &start, &end);
  buffer_text = gtk_text_buffer_get_text (heading, &start, &end, FALSE);

  ASSERT_NE (heading, nullptr);
  ASSERT_TRUE (GTK_IS_TEXT_BUFFER (heading));
  EXPECT_STREQ (buffer_text, COLUMN_HEADING);

  g_free (buffer_text);
}

TEST_F (KanbanColumnObservableInterfaceTests,
        GetId_ValidKanbanColumnViewModelPassed_ColumnIdReturned)
{
  KanbanColumnObservable *column_iface = KANBAN_COLUMN_OBSERVABLE (viewmodel);
  int column_id = kanban_column_observable_get_id (column_iface);
  EXPECT_EQ (column_id, COLUMN_ID);
}
