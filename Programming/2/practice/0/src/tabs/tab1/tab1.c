#include "tab1.h"

struct _Tab1 {
    GtkGrid parent_instance;
    GtkEntry *entry;
    GtkLabel *label;
};

G_DEFINE_TYPE(Tab1, tab1, GTK_TYPE_GRID)

static void on_button_clicked(gpointer user_data) {
    Tab1 *self = (Tab1*)user_data;
    gtk_label_set_text(self->label, gtk_entry_buffer_get_text (gtk_entry_get_buffer (self->entry)));
}

static void tab1_init(Tab1 *self) {
    gtk_widget_init_template(GTK_WIDGET (self));
}

static void tab1_class_init(Tab1Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab1.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab1, entry);
    gtk_widget_class_bind_template_child(widget_class, Tab1, label);
    gtk_widget_class_bind_template_callback(widget_class, on_button_clicked);
}

GtkWidget *tab1_new (void) {
    return g_object_new(TYPE_TAB1, NULL);
}
