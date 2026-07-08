#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB3 (tab3_get_type ())
G_DECLARE_FINAL_TYPE(Tab3, tab3, TAB3, MAIN, GtkBox)

GtkWidget *tab3_new(void);
