#ifndef KANBAN_APP_H
#define KANBAN_APP_H

#include <gtk/gtk.h>


#define KANBAN_APP_TYPE (kanban_app_get_type ())

G_DECLARE_FINAL_TYPE (KanbanApp, kanban_app, KANBAN, APP, GtkApplication)


KanbanApp     *kanban_app_new         (void);


#endif /* KANBAN_APP_H */
