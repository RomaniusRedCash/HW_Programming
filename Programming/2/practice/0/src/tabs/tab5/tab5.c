#include "tab5.h"

struct _Tab5 {
    GtkBox parent_instance;
    GtkEntry *entry_x1;
    GtkEntry *entry_y1;
    GtkEntry *entry_x2;
    GtkEntry *entry_y2;
    GtkWidget *drawing_area;

    int m_p[4];
};

G_DEFINE_TYPE(Tab5, tab5, GTK_TYPE_BOX)

static void on_drawing_area_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data) {
    Tab5 *self = (Tab5*)user_data;
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
    cairo_set_line_width(cr, 2.0);
    cairo_move_to(cr, self->m_p[0], self->m_p[1]);
    cairo_line_to(cr, self->m_p[2], self->m_p[3]);
    cairo_stroke(cr);
}

static void on_draw_button_clicked(gpointer user_data) {
    Tab5 *self = (Tab5*)user_data;
    self->m_p[0] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(self->entry_x1)));
    self->m_p[1] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(self->entry_y1)));
    self->m_p[2] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(self->entry_x2)));
    self->m_p[3] = atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(self->entry_y2)));
    gtk_widget_queue_draw(self->drawing_area);
    int height = abs(self->m_p[0] - self->m_p[2]), width = abs(self->m_p[1] - self->m_p[3]), max_size = height > width ? height : width;
    if (max_size > 720) {
        for (char i = 0; i < 4; i+=2) {
            self->m_p[i] = self->m_p[i] * 720 / max_size;
            self->m_p[i+1] = self->m_p[i+1] * 720 / max_size;
        }
        max_size=720;
    }
    gtk_widget_set_size_request(self->drawing_area, max_size, max_size);
}

static void tab5_init(Tab5 *self) {
    gtk_widget_init_template(GTK_WIDGET (self));
    for(char i = 0; i < 4; i++) self->m_p[i] = 0;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->drawing_area), on_drawing_area_draw, self, NULL);
}

static void tab5_class_init(Tab5Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab5.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab5, entry_x1);
    gtk_widget_class_bind_template_child(widget_class, Tab5, entry_y1);
    gtk_widget_class_bind_template_child(widget_class, Tab5, entry_x2);
    gtk_widget_class_bind_template_child(widget_class, Tab5, entry_y2);
    gtk_widget_class_bind_template_child(widget_class, Tab5, drawing_area);
    gtk_widget_class_bind_template_callback(widget_class, on_draw_button_clicked);
}

GtkWidget *tab5_new (void) {
    return g_object_new(TYPE_TAB5, NULL);
}
