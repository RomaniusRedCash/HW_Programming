#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB2 (tab2_get_type ())
G_DECLARE_FINAL_TYPE(Tab2, tab2, TAB2, MAIN, GtkGrid)

GtkWidget *tab2_new(void);
