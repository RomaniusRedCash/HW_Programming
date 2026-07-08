#include "tab6.h"

struct _Tab6 {
    GtkBox parent_instance;
    GtkLabel* label;

    gboolean is_activate;
};

G_DEFINE_TYPE(Tab6, tab6, GTK_TYPE_BOX)

static void on_label_mouse_hover(GtkEventControllerMotion *controller, double x, double y, gpointer user_data) {
    Tab6 *self = (Tab6*)user_data;
    if (self->is_activate) return;
    self->is_activate = TRUE;

    gtk_label_set_text(GTK_LABEL(self->label), "Z");
    gtk_widget_add_css_class(GTK_WIDGET(self->label), "custom-label");

    GtkAlertDialog *dialog = gtk_alert_dialog_new("Написано же\nНЕ трогать!");
    gtk_alert_dialog_show(dialog, GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self))));
    g_object_unref(dialog);
}


static void tab6_init(Tab6 *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
}

static void tab6_class_init(Tab6Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab6.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab6, label);
    gtk_widget_class_bind_template_callback(widget_class, on_label_mouse_hover);
}

GtkWidget *tab6_new (void) {
    return g_object_new(TYPE_TAB6, NULL);
}
