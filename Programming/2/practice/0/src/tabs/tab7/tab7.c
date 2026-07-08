#include "tab7.h"

#include <math.h>

typedef struct {
    int x;
    int y;
} MyPoint;

struct _Tab7 {
    GtkBox parent_instance;
    GtkWidget *drawing_area;
    GtkButton* button;
    double angle;
    int current_points_count;
    MyPoint p[64];

    guint timer_id;
    gboolean is_runnung;
};

G_DEFINE_TYPE(Tab7, tab7, GTK_TYPE_BOX)

static void on_drawing_area_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data) {
    Tab7 *self = (Tab7*)user_data;
    int cX = 120, cY = 120;
    int R2 = 90;
    double k = 20.0;
    double R1 = (int)(R2 / k);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    if (self->current_points_count > 1) {
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); // Красный
        cairo_set_line_width(cr, 1.5);
        cairo_move_to(cr, self->p[0].x, self->p[0].y);
        for (int j = 1; j < self->current_points_count; j++) {
            cairo_line_to(cr, self->p[j].x, self->p[j].y);
        }
        cairo_stroke(cr);
    }
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 1.0);
    cairo_arc(cr, cX, cY, R2, 0, 2 * M_PI);
    cairo_stroke(cr);
    double x = R1 * (k - 1) * (cos(self->angle) + cos((k - 1) * self->angle) / (k - 1));
    double y = R1 * (k - 1) * (sin(self->angle) - sin((k - 1) * self->angle) / (k - 1));
    double x1 = (R2 - R1) * sin(self->angle + 1.57);
    double y1 = (R2 - R1) * cos(self->angle + 1.57);
    double centX = cX + x1;
    double centY = cY - y1;
    cairo_arc(cr, centX, centY, R1, 0, 2 * M_PI);
    cairo_stroke(cr);
    cairo_move_to(cr, centX, centY);
    cairo_line_to(cr, cX + x, cY + y);
    cairo_stroke(cr);
}

static gboolean on_animation_tick(gpointer user_data) {
    Tab7 *self = (Tab7*)user_data;
    double LastT = 6.3;
    double Step = 0.1;
    int cX = 120, cY = 120;
    int R2 = 90;
    double k = 20.0;
    double R1 = (int)(R2 / k);
    if (self->angle <= LastT && self->current_points_count < 64) {
        double x = R1 * (k - 1) * (cos(self->angle) + cos((k - 1) * self->angle) / (k - 1));
        double y = R1 * (k - 1) * (sin(self->angle) - sin((k - 1) * self->angle) / (k - 1));
        self->p[self->current_points_count].x = cX + (int)x;
        self->p[self->current_points_count].y = cY + (int)y;
        self->current_points_count++;
        self->angle += Step;
        gtk_widget_queue_draw(self->drawing_area);
        return G_SOURCE_CONTINUE;
    } else {
        self->timer_id = 0;
        return G_SOURCE_REMOVE;
    }
}

static void on_button_clicked(gpointer user_data) {
    Tab7 *self = (Tab7*)user_data;
    if (self->is_runnung) return;
    self->is_runnung = TRUE;
    self->timer_id = g_timeout_add(40, on_animation_tick, self);
}

static void tab7_init(Tab7 *self) {
    gtk_widget_init_template(GTK_WIDGET (self));
    self->angle = 0.0;
    self->current_points_count = 0;
    self->is_runnung=FALSE;
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->drawing_area), on_drawing_area_draw, self, NULL);
}

static void tab7_class_init(Tab7Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab7.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab7, drawing_area);
    gtk_widget_class_bind_template_child(widget_class, Tab7, button);
    gtk_widget_class_bind_template_callback(widget_class, on_button_clicked);
}

GtkWidget *tab7_new (void) {
    return g_object_new(TYPE_TAB7, NULL);
}
