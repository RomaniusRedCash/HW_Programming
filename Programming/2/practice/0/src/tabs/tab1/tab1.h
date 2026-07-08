#pragma once
#include <gtk/gtk.h>
#include "../tabs.h"

#define TYPE_TAB1 (tab1_get_type ())
G_DECLARE_FINAL_TYPE(Tab1, tab1, TAB1, MAIN, GtkGrid)

GtkWidget *tab1_new(void);
