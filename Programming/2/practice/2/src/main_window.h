#pragma once

#include <gtk/gtk.h>

#define TYPE_MAIN_WINDOW (main_window_get_type())

G_DECLARE_FINAL_TYPE(MainWindow, main_window, MAIN, WINDOW, GtkApplicationWindow)

GtkWidget *main_window_new(GtkApplication *app);
