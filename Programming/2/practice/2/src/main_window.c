#include "main_window.h"

#include "header.h"
#include "tree.h"

struct _MainWindow {
    GtkApplicationWindow parent_instance;
    GtkWidget *drawing_area, *drawing_area_tree;
    GtkButton *button_plus, *button_minus;
    GtkLabel *depth_label;
    int depth;
    tree *tr;
};

typedef struct {
    double r, g, b;
} color;

#define set_color(color) cairo_set_source_rgb(cr, color.r, color.g, color.b);
static const color clr[7] = {{0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}};
static void draw_rec(cairo_t *cr, node *n) {
    color c = clr[n->level % 6];
    cairo_move_to(cr, n->points[0].x, n->points[0].y);
    for (int i = 1; i < 4; i++)
        cairo_line_to(cr, n->points[i].x, n->points[i].y);
    cairo_close_path(cr);
    cairo_set_line_width(cr, 1.5);
    set_color(c);
    cairo_fill_preserve(cr);
    c = clr[(n->level + 2) % 6];
    set_color(c);
    cairo_stroke(cr);
}

static void draw_recursive(MainWindow* self, cairo_t *cr, node *n) {
    if (n->level > self->depth) return;
    cairo_save(cr);
    cairo_translate(cr, 0, self->tr->root->points[2].y + 50);
    for (int i = 0; i < n->num_sub; i++)
        draw_recursive(self, cr, n->sub[i]);
    cairo_restore(cr);
    draw_rec(cr, n);
}

static void on_drawing_area_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data) {
    MainWindow *self = MAIN_WINDOW(user_data);
    gtk_drawing_area_set_content_height(drawing_area, self->depth * (self->tr->root->points[2].y + 50));
    cairo_save(cr);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    draw_recursive(self, cr, self->tr->root);
    cairo_restore(cr);
}

void draw_node_recursive(cairo_t *cr, node *curr) {
    if (curr == NULL) return;
    if (curr->parent != NULL) {
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_move_to(cr, curr->parent->c_n.x, curr->parent->c_n.y);
        cairo_line_to(cr, curr->c_n.x, curr->c_n.y);
        cairo_stroke(cr);
    }
    cairo_new_sub_path(cr);
    cairo_arc(cr, curr->c_n.x, curr->c_n.y, 5, 0, 2 * G_PI);
    color c = clr[(curr->level + 0) % 6];
    set_color(c);
    cairo_fill_preserve(cr);
    set_color(clr[(curr->level + 2) % 6]);
    cairo_stroke(cr);
    for (int i = 0; i < curr->num_sub; i++) {
        draw_node_recursive(cr, curr->sub[i]);
    }
}

static void on_drawing_area_tree_draw(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data) {
    MainWindow *self = MAIN_WINDOW(user_data);
    gtk_drawing_area_set_content_height(drawing_area, self->depth * (self->tr->root->points[2].y + 50));
    cairo_save(cr);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    draw_node_recursive(cr, self->tr->root);
    cairo_restore(cr);
}
#undef set_color

void calculate_node_coordinates(node *curr, double x, double y, double width) {
    if (curr == NULL) return;
    curr->c_n.x = x;
    curr->c_n.y = y;
    if (curr->num_sub > 0) {
        double sub_width = width / curr->num_sub;
        double start_x = x - width / 2.0 + sub_width / 2.0;
        for (int i = 0; i < curr->num_sub; i++) {
            if (curr->sub[i] != NULL) {
                double child_x = start_x + (i * sub_width);
                double child_y = y + 350;
                calculate_node_coordinates(curr->sub[i], child_x, child_y, sub_width);
            }
        }
    }
}

G_DEFINE_TYPE(MainWindow, main_window, GTK_TYPE_APPLICATION_WINDOW)

static void change_param(GtkButton *button, gpointer user_data) {
    MainWindow *self = MAIN_WINDOW(user_data);
    if (button == self->button_plus) {
        self->depth++;
        if (self->depth >= 15) gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(self->button_minus), TRUE);
        if (self->depth > self->tr->level) tree_increase(self->tr);
    } else {
        self->depth--;
        if (self->depth <= 1)
            gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(self->button_plus), TRUE);
    }
    char str[256];
    sprintf(str, "%d", self->depth);
    gtk_label_set_text(self->depth_label, str);
    gtk_widget_queue_draw(self->drawing_area);
    gtk_widget_queue_draw(self->drawing_area_tree);
    calculate_node_coordinates(self->tr->root, 600.0/2, 350.0/2, 600);
}

static void main_window_finalize(GObject *object) {
    MainWindow *self = MAIN_WINDOW(object);
    tree_clear(self->tr);
    G_OBJECT_CLASS(main_window_parent_class)->finalize(object);
}

static void main_window_class_init(MainWindowClass *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
    G_OBJECT_CLASS(klass)->finalize = main_window_finalize;
    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(builder.ui));
    gtk_widget_class_bind_template_child(widget_class, MainWindow, drawing_area);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, drawing_area_tree);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, button_plus);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, button_minus);
    gtk_widget_class_bind_template_child(widget_class, MainWindow, depth_label);
    gtk_widget_class_bind_template_callback(widget_class, change_param);
}

static void main_window_init(MainWindow *self) {
    gtk_widget_init_template(GTK_WIDGET(self));
    self->depth = 1;
    self->tr = tree_new();
    gtk_widget_set_sensitive(GTK_WIDGET(self->button_minus), FALSE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->drawing_area), on_drawing_area_draw, self, NULL);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(self->drawing_area_tree), on_drawing_area_tree_draw, self, NULL);
}

GtkWidget *main_window_new(GtkApplication *app) {
    return g_object_new(main_window_get_type(), "application", app, NULL);
}
