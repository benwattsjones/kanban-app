/* tests/application-entry-tests.cpp
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
  #include "../src/presenters/kanban-application.h"
  #include "../src/presenters/kanban-list-store.h"
  #include <config.h>

  #include <gtk/gtk.h>
}

#include "gtest/gtest.h"

// Stubs:
extern "C"
{
  void initialize_kanban_view (KanbanApplication *app) { (void) app; }
  KanbanListStore *initialize_viewmodel() { return NULL; }
  void destroy_viewmodel (KanbanListStore *viewmodel) { (void) viewmodel; }
}

// Test Fixtures:
class ApplicationEntryTests : public ::testing::Test
{
protected:
  gpointer app;
  static gchar *printed_string;

  static void redirect_gprint(const gchar *string)
  {
    g_free (printed_string);
    printed_string = g_strdup (string);
  }

  void SetUp() override
  {
    printed_string = NULL;
    g_set_print_handler (redirect_gprint);
    app = g_object_new (KANBAN_APPLICATION_TYPE,
                        "application-id", APPLICATION_ID,
                        "flags", G_APPLICATION_HANDLES_OPEN,
                        NULL);
  }

  void TearDown() override
  {
    g_object_unref (app);
    g_free (printed_string);
  }
};

gchar *ApplicationEntryTests::printed_string = NULL;

// Tests:
TEST_F(ApplicationEntryTests, checkExitCodeSuccess)
{
  int status = g_application_run (G_APPLICATION (app), 0, NULL);
  EXPECT_EQ(status, 0);
}

TEST_F(ApplicationEntryTests, checkVersionOptionPrints)
{
  char *prog_name = g_strdup("prog");
  char *cmd_options = g_strdup("--version");
  char *opt[] = {prog_name, cmd_options};
  int status = g_application_run (G_APPLICATION (app), 2, opt);
  EXPECT_EQ(status, 0);
  EXPECT_STREQ(printed_string, PACKAGE_NAME " " PACKAGE_VERSION "\n");
  g_free(prog_name);
  g_free(cmd_options);
}

