#include "tab2.h"

struct _Tab2 {
    GtkGrid parent_instance;
    GtkEntry *entry;
    GtkLabel *label;
};

G_DEFINE_TYPE(Tab2, tab2, GTK_TYPE_GRID)

static void on_button_clicked(gpointer user_data) {
    Tab2 *self = (Tab2 *)user_data;
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self)));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    GtkWidget *dialog = gtk_message_dialog_new(parent, GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Здравствуй, %s!", gtk_entry_buffer_get_text(gtk_entry_get_buffer(self->entry)));
#pragma GCC diagnostic pop
    gtk_window_set_title (GTK_WINDOW(dialog), "Приветствие");
    gtk_window_present (GTK_WINDOW(dialog));
    g_signal_connect (dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
}

static void tab2_init(Tab2 *self) {
    gtk_widget_init_template(GTK_WIDGET (self));
}

static void tab2_class_init(Tab2Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab2.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab2, entry);
    gtk_widget_class_bind_template_child(widget_class, Tab2, label);
    gtk_widget_class_bind_template_callback(widget_class, on_button_clicked);
}

GtkWidget *tab2_new(void) {
    return g_object_new(TYPE_TAB2, NULL);
}
