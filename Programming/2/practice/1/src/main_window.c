#include "main_window.h"

#include <math.h>

#include "header.h"


struct _MainWindow {
    GtkApplicationWindow parent_instance;
    GtkWidget* drawing_area;
    GtkSwitch* napr_switcher;
    GtkSwitch* orb_switcher;

#define SCALE_LIST \
    X(a_param)      \
    X(diag1)     \
    X(diag2)  \
    X(speed_orb) \
    X(speed_trajectory) \
    X(ampl) \
    X(speed_ampl) \
    X(obvodka)

#define X(name) GtkScale* name##_scale; double name;
    SCALE_LIST
#undef X

#define COLOR_LIST \
    X(contur)      \
    X(zalivka)     \
    X(trajectory)  \
    X(sub_contur)

#define X(name) GtkColorDialogButton* color_##name##_button; const GdkRGBA* color_##name;
    COLOR_LIST
#undef X

    guint timer_id;
    int16_t cur_deg;
    double cur_apml;
    gboolean obratno;
    gboolean orb;
};

G_DEFINE_TYPE(MainWindow, main_window, GTK_TYPE_APPLICATION_WINDOW)

static void parce_param(gpointer user_data) {
    MainWindow* self = (MainWindow*)user_data;
    self->obratno = gtk_switch_get_state(self->napr_switcher);
    self->orb = gtk_switch_get_state(self->orb_switcher);

#define X(name) self->name = gtk_range_get_value(GTK_RANGE(self->name##_scale));
    SCALE_LIST
#undef X

#define X(name) self->color_##name = gtk_color_dialog_button_get_rgba(self->color_##name##_button);
    COLOR_LIST
#undef X
}

static void on_drawing_area_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data) {
#define set_color(cr, color) cairo_set_source_rgb(cr, self->color->red, self->color->green, self->color->blue);

    MainWindow* self = (MainWindow*)user_data;

    static const double dash_pattern[] = {8.0, 4.0};
    static const int num_dashes = 2;

    double centerx = width / 2.0;
    double centery = height / 2.0;

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

    cairo_save(cr);
    cairo_translate(cr, centerx, centery);

    set_color(cr, color_sub_contur);
    cairo_set_line_width(cr, 1.5);

    cairo_set_dash(cr, dash_pattern, num_dashes, 0.0);
    cairo_arc(cr, 0, 0, self->a_param, 0, 2 * G_PI);
    cairo_stroke(cr);
    cairo_set_dash(cr, NULL, 0, 0.0);

    double phi = 0;
    double r;
    double x;
    double y;

    set_color(cr, color_trajectory);

    for (int i = 0; i <= 360; i++) {
        phi = i * G_PI / 180.0;
        r = self->a_param * sin(3.0 * phi);
        x = r * sin(phi);
        y = r * cos(phi);
        if (i == 0) cairo_move_to(cr, x, y);
        else cairo_line_to(cr, x, y);
    }
    cairo_stroke(cr);

    if (self->orb) {
// ТРАЕКТОРИЯ
        phi = self->cur_deg * G_PI / 180.0 * self->speed_trajectory;
        r = self->a_param * sin(3.0 * phi);
        x = r * sin(phi);
        y = r * cos(phi);
        cairo_translate(cr, x, y);

        set_color(cr, color_sub_contur);
        cairo_set_line_width(cr, 1.5);
        cairo_arc(cr, 0, 0, 5, 0, 2 * G_PI);
        cairo_fill(cr);
// ОРБИТА
        phi = self->cur_deg * G_PI / 180.0 * self->speed_orb;
        r = self->a_param / 2.0;
        x = r * sin(phi);
        y = r * cos(phi);
        set_color(cr, color_sub_contur);
        cairo_set_line_width(cr, 1.5);
        cairo_set_dash(cr, dash_pattern, num_dashes, 0.0);
        cairo_arc(cr, 0, 0, r, 0, 2 * G_PI);
        cairo_stroke(cr);
        cairo_set_dash(cr, NULL, 0, 0.0);

        cairo_translate(cr, x, y);
        cairo_rotate(cr, -phi);
        if (self->ampl != 0.0) {
            double rad = self->cur_deg * G_PI * self->speed_ampl / 180.0;
            self->cur_apml = self->ampl * sin(rad);
        } else self->cur_apml = 0.0;
        double scale = 1.0 + self->cur_apml;
        cairo_scale(cr, scale, scale);
        cairo_set_line_width(cr, self->obvodka);
        double sub_diag1 = self->diag1 / 2.0;
        double sub_diag2 = self->diag2 / 2.0;
        cairo_move_to(cr, sub_diag1, 0);
        cairo_line_to(cr, 0, sub_diag2);
        cairo_line_to(cr, -sub_diag1, 0);
        cairo_line_to(cr, 0, -sub_diag2);
        cairo_line_to(cr, sub_diag1, 0);
        set_color(cr, color_zalivka);
        cairo_fill_preserve(cr);
        set_color(cr, color_contur);
        cairo_stroke(cr);
    } else {
// ТРАЕКТОРИЯ ДЛЯ ОРБИТЫ
        cairo_save(cr);
        phi = self->cur_deg * G_PI * self->speed_orb / 180.0;
        r = self->a_param * sin(3.0 * phi);
        x = r * sin(phi);
        y = r * cos(phi);
        cairo_translate(cr, x, y);

        if (self->ampl != 0.0) {
            double rad = self->cur_deg * G_PI * self->speed_ampl / 180.0;
            self->cur_apml = self->ampl * sin(rad);
        } else self->cur_apml = 0.0;
        double scale = 1.0 + self->cur_apml;
        cairo_scale(cr, scale, scale);

        cairo_set_line_width(cr, self->obvodka);
        cairo_arc(cr, 0, 0, self->a_param, 0, 2 * G_PI);
        set_color(cr, color_zalivka);
        cairo_fill_preserve(cr);
        set_color(cr, color_contur);
        cairo_stroke(cr);
        cairo_restore(cr);

// ТРАЕКТОРИЯ ДЛЯ РОМБА
        phi = self->cur_deg * G_PI / 180.0 * self->speed_trajectory;
        r = self->a_param * sin(3.0 * phi);
        x = r * sin(phi);
        y = r * cos(phi);
        cairo_translate(cr, x, y);
        cairo_rotate(cr, -phi);
        cairo_scale(cr, scale, scale);
        cairo_set_line_width(cr, self->obvodka);
        double sub_diag1 = self->diag1 / 2.0;
        double sub_diag2 = self->diag2 / 2.0;
        cairo_move_to(cr, sub_diag1, 0);
        cairo_line_to(cr, 0, sub_diag2);
        cairo_line_to(cr, -sub_diag1, 0);
        cairo_line_to(cr, 0, -sub_diag2);
        cairo_line_to(cr, sub_diag1, 0);
        set_color(cr, color_zalivka);
        cairo_fill_preserve(cr);
        set_color(cr, color_contur);
        cairo_stroke(cr);
        cairo_restore(cr);
    }

    cairo_restore(cr);
#undef set_color
}

static gboolean on_animation_tick(gpointer user_data) {
    MainWindow* self = (MainWindow*)user_data;
    if (self->obratno) self->cur_deg++;
    else self->cur_deg--;
    self->cur_deg%=360;
    gtk_widget_queue_draw(self->drawing_area);
    return G_SOURCE_CONTINUE;
}

static void main_window_class_init(MainWindowClass* klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(builder.ui));
    gtk_widget_class_bind_template_child(widget_class, MainWindow, drawing_area);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, napr_switcher);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, orb_switcher);

#define X(name) gtk_widget_class_bind_template_child(widget_class, MainWindow, name##_scale);
    SCALE_LIST
#undef X

#define X(name) gtk_widget_class_bind_template_child(widget_class, MainWindow, color_##name##_button);
    COLOR_LIST
#undef X

    gtk_widget_class_bind_template_callback(widget_class, parce_param);
}

static void main_window_init(MainWindow *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
    self->cur_deg=0;
    self->cur_apml = 0;
    parce_param(self);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->drawing_area), on_drawing_area_draw, self, NULL);
    self->timer_id = g_timeout_add(40, on_animation_tick, self);
}

GtkWidget* main_window_new(GtkApplication* app) {
    return g_object_new(main_window_get_type(), "application", app, NULL);
}
