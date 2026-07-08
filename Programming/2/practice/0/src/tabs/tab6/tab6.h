#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB6 (tab6_get_type())
G_DECLARE_FINAL_TYPE(Tab6, tab6, TAB6, MAIN, GtkBox)

GtkWidget *tab6_new(void);
