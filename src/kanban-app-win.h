#ifndef KANBAN_APP_WIN_H
#define KANBAN_APP_WIN_H

#include <gtk/gtk.h>
#include "kanban-app.h"


#define KANBAN_APP_WINDOW_TYPE (kanban_app_window_get_type ())
G_DECLARE_FINAL_TYPE (KanbanAppWindow, kanban_app_window, KANBAN, APP_WINDOW, GtkApplicationWindow)


KanbanAppWindow        *kanban_app_window_new          (KanbanApp *app);
void                    kanban_app_window_open         (KanbanAppWindow *win,
                                                         GFile            *file);


#endif /* KANBAN_APP_WIN_H */
