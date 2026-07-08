#include "tab3.h"

struct _Tab3 {
    GtkBox parent_instance;
    GtkPicture* img;
    GtkDropDown* dd;
    GtkLabel* label;
    GtkLabel* ltext;
    GtkOverlay* overlay;
    GtkAspectFrame* aspect_frame;
    GtkTextView* txt_view;
    GtkButton* button;
};

G_DEFINE_TYPE(Tab3, tab3, GTK_TYPE_BOX)

static void on_button_clicked(gpointer user_data) {
    Tab3 *self = (Tab3*)user_data;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->txt_view);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    char *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
    gtk_label_set_text(self->ltext, text);
    g_free(text);
}

static void update_label_from_dropdown(Tab3 *self) {
    const char* list_label[] = {"абдулоикона", "svidetel iz friazino", "предусилитель", "епифанцев"};
    const char* list_img[] = {DEFAULT_IMG_PATH(абдулоикона.jpg),
        DEFAULT_IMG_PATH(svidetel iz friazino.png),
        DEFAULT_IMG_PATH(предусилитель.PNG),
        DEFAULT_IMG_PATH(епифанцев.png)
    };
    guint selected = gtk_drop_down_get_selected(self->dd);
    gtk_label_set_text(self->label, list_label[selected]);
    gtk_picture_set_resource(self->img, list_img[selected]);
    GdkTexture *texture = gtk_picture_get_paintable(self->img) ?
    GDK_TEXTURE(gtk_picture_get_paintable(self->img)) : NULL;
    if (texture) {
        float width = gdk_texture_get_width(texture);
        float height = gdk_texture_get_height(texture);
        if (height > 0) {
            gtk_aspect_frame_set_ratio(self->aspect_frame, width / height);
        }
    }
}

static void on_activate(gpointer user_data) {
    Tab3 *self = (Tab3*)user_data;
    update_label_from_dropdown(self);
}

static void tab3_init(Tab3 *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
    update_label_from_dropdown(self);
}

static void tab3_class_init(Tab3Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab3.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab3, img);
    gtk_widget_class_bind_template_child(widget_class, Tab3, dd);
    gtk_widget_class_bind_template_child(widget_class, Tab3, label);
    gtk_widget_class_bind_template_child(widget_class, Tab3, ltext);
    gtk_widget_class_bind_template_child(widget_class, Tab3, overlay);
    gtk_widget_class_bind_template_child(widget_class, Tab3, aspect_frame);
    gtk_widget_class_bind_template_child(widget_class, Tab3, txt_view);
    gtk_widget_class_bind_template_child(widget_class, Tab3, button);
    gtk_widget_class_bind_template_callback(widget_class, on_button_clicked);
    gtk_widget_class_bind_template_callback(widget_class, on_activate);

}

GtkWidget *tab3_new(void) {
    return g_object_new(TYPE_TAB3, NULL);
}
