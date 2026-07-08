#include "tab4.h"

struct _Tab4 {
    GtkBox parent_instance;
    GtkTextView *text_view;
    GFile *current_file;
    gboolean is_modified;
};

G_DEFINE_TYPE(Tab4, tab4, GTK_TYPE_BOX)

static void on_menu_open_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);
static void on_menu_save_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data);

static void on_text_changed(GtkTextBuffer *buffer, gpointer user_data) {
    Tab4* self = (Tab4*)user_data;
    self->is_modified = TRUE;
}

static void tab4_init_actions(Tab4 *self) {
    GSimpleActionGroup *action_group = g_simple_action_group_new();

    GSimpleAction *action = g_simple_action_new("open", NULL);
    g_signal_connect(action, "activate", G_CALLBACK(on_menu_open_activated), self);
    g_action_map_add_action(G_ACTION_MAP(action_group), G_ACTION(action));
    g_object_unref(action);

    action = g_simple_action_new("save_as", NULL);
    g_signal_connect(action, "activate", G_CALLBACK(on_menu_save_activated), self);
    g_action_map_add_action(G_ACTION_MAP(action_group), G_ACTION(action));
    g_object_unref(action);

    gtk_widget_insert_action_group(GTK_WIDGET(self), "tab4", G_ACTION_GROUP(action_group));
    g_object_unref(action_group);
}

static void tab4_init(Tab4 *self) {
    gtk_widget_init_template(GTK_WIDGET(self));

    self->current_file = NULL;
    self->is_modified = FALSE;

    tab4_init_actions(self);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->text_view);
    g_signal_connect(buffer, "changed", G_CALLBACK(on_text_changed), self);
}

static void tab4_class_init(Tab4Class *klass) {
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);

    gtk_widget_class_set_template_from_resource(widget_class, DEFAULT_UI_PATH(tab4.ui));
    gtk_widget_class_bind_template_child(widget_class, Tab4, text_view);
}

GtkWidget *tab4_new (void) {
    return g_object_new(TYPE_TAB4, NULL);
}

static void add_filter(GListStore *filters, const char* name, const char* param) {
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, name);
    gtk_file_filter_add_pattern(filter, param);

    g_list_store_append(filters, filter);
    g_object_unref(filter);
}

static void on_open_dialog_finish(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    Tab4* self = (Tab4*)user_data;

    GError *error = NULL;
    GFile *file = gtk_file_dialog_open_finish(dialog, res, &error);

    if (file != NULL) {
        char *contents = NULL;
        gsize length = 0;

        if (g_file_load_contents(file, NULL, &contents, &length, NULL, &error)) {
            GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->text_view);

            g_signal_handlers_block_by_func(buffer, G_CALLBACK(on_text_changed), self);
            gtk_text_buffer_set_text(buffer, contents, length);
            g_signal_handlers_unblock_by_func(buffer, G_CALLBACK(on_text_changed), self);

            if (self->current_file) g_object_unref(self->current_file);
            self->current_file = g_object_ref(file);
            self->is_modified = FALSE;

            g_free(contents);
        } else {
            g_printerr("Ошибка чтения файла: %s\n", error->message);
            g_clear_error(&error);
        }
        g_object_unref(file);
    } else {
        if (error) {
            g_clear_error(&error);
        }
    }
}

static void run_open_file_dialog(Tab4 *self);

static void on_save_prompt_response(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GtkAlertDialog *alert = GTK_ALERT_DIALOG(source_object);
    Tab4* self = (Tab4*)user_data;
    GError *error = NULL;
    int response = gtk_alert_dialog_choose_finish(alert, res, &error);
    if (error) {
        g_clear_error(&error);
        return;
    }
    switch (response) {
        case 0:
            on_menu_save_activated(NULL, NULL, self);
            break;
        case 1:
            run_open_file_dialog(self);
            break;
        case 2:
        default:
            break;
    }
}

static void warning_modified(Tab4 *self, GAsyncReadyCallback response) {
    GtkAlertDialog *alert = gtk_alert_dialog_new("Текст был изменен.");
    const char *buttons[] = { "Сохранить", "Не сохранять", "Отмена", NULL };
    gtk_alert_dialog_set_buttons(alert, buttons);
    gtk_alert_dialog_set_default_button(alert, 0);
    gtk_alert_dialog_set_cancel_button(alert, 2);
    gtk_alert_dialog_choose(alert, GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self))), NULL, response, self);
    g_object_unref(alert);
}

static void on_menu_open_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    Tab4* self = (Tab4*)user_data;
    if (self->is_modified) {
        warning_modified(self, on_save_prompt_response);
    } else {
        run_open_file_dialog(self);
    }
}

static void run_open_file_dialog(Tab4 *self) {
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Открыть файл");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    add_filter(filters, "Текстовые файлы (*.txt)", "*.txt");
    add_filter(filters, "Все файлы (*)", "*");
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    g_object_unref(filters);

    gtk_file_dialog_open(dialog, GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self))), NULL, on_open_dialog_finish, self);
    g_object_unref(dialog);
}

static void on_save_dialog_finish(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    Tab4* self = (Tab4*)user_data;

    GError *error = NULL;
    GFile *file = gtk_file_dialog_save_finish(dialog, res, &error);

    if (file != NULL) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->text_view);
        GtkTextIter start, end;

        gtk_text_buffer_get_bounds(buffer, &start, &end);
        char *contents = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        if (g_file_replace_contents(file, contents, strlen(contents), NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL, &error)) {
            if (self->current_file) g_object_unref(self->current_file);
            self->current_file = g_object_ref(file);
            self->is_modified = FALSE;
        } else {
            g_printerr("Ошибка сохранения файла: %s\n", error->message);
            g_clear_error(&error);
        }
        g_free(contents);
        g_object_unref(file);
    } else {
        if (error) {
            g_clear_error(&error);
        }
    }
}

static void on_menu_save_activated(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    Tab4* self = (Tab4*)user_data;

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(dialog, "Сохранить файл");
    gtk_file_dialog_set_initial_file(dialog, self->current_file);

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    add_filter(filters, "Текстовые файлы (*.txt)", "*.txt");
    add_filter(filters, "Все файлы (*)", "*");
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    g_object_unref(filters);

    gtk_file_dialog_save(dialog, GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(self))), NULL, on_save_dialog_finish, self);
    g_object_unref(dialog);
}

static void on_main_window_exit_response(GObject *source_object, GAsyncResult *res, gpointer user_data) {
    Tab4* self = (Tab4*)user_data;
    GtkAlertDialog *alert = GTK_ALERT_DIALOG(source_object);
    GtkRoot *root = gtk_widget_get_root(GTK_WIDGET(self));
    GError *error = NULL;
    int response = gtk_alert_dialog_choose_finish(alert, res, &error);
    if (error) {
        g_clear_error(&error);
        return;
    }
    switch (response) {
        case 0:
            on_menu_save_activated(NULL, NULL, self);
            break;
        case 1:
            gtk_window_destroy(GTK_WINDOW(root));
            break;
        case 2:
        default:
            break;
    }
}

gboolean tab4_on_main_window_close_requested(GtkWindow *window, gpointer user_data) {
    Tab4* self = (Tab4*)user_data;
    if (self->is_modified) {
        warning_modified(self, on_main_window_exit_response);
        return TRUE;
    }
    return FALSE;
}
