#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB7 (tab7_get_type ())
G_DECLARE_FINAL_TYPE(Tab7, tab7, TAB7, MAIN, GtkBox)

GtkWidget *tab7_new(void);
