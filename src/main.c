#include <gtk/gtk.h>

#include "kanban-app.h"


int
main (int argc, char *argv[])
{
  return g_application_run (G_APPLICATION (kanban_app_new ()), argc, argv);
}

