#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB4 (tab4_get_type ())
G_DECLARE_FINAL_TYPE(Tab4, tab4, TAB4, MAIN, GtkBox)

GtkWidget *tab4_new(void);

gboolean tab4_on_main_window_close_requested(GtkWindow *window, gpointer user_data);
