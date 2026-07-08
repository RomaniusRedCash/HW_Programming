#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB5 (tab5_get_type ())
G_DECLARE_FINAL_TYPE(Tab5, tab5, TAB5, MAIN, GtkBox)

GtkWidget *tab5_new(void);
