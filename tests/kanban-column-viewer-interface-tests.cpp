/* tests/kanban-column-viewer-interface-tests.cpp
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
  #include "../src/presenters/kanban-column-viewer-interface.h"

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Test Fixtures:
class KanbanColumnViewerInterfaceTests : public ::testing::Test
{
protected:

  void SetUp() override
  {
  }

  void TearDown() override
  {
  }
};

// Tests:
TEST_F(KanbanColumnViewerInterfaceTests, checkNullDoesntCauseError)
{
  kanban_column_viewer_add_column (NULL, NULL);
  SUCCEED();
}


