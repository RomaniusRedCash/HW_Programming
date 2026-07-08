#include <gtk/gtk.h>
#include <glib/gstdio.h>

#define STRINGIFY(x) #x
#define ADD_TAB_H(A) STRINGIFY(tabs/tab##A/tab##A.h)

#include ADD_TAB_H(1)
#include ADD_TAB_H(2)
#include ADD_TAB_H(3)
#include ADD_TAB_H(4)
#include ADD_TAB_H(5)
#include ADD_TAB_H(6)
#include ADD_TAB_H(7)

static void create_new_tab(GtkWidget* tab, GtkNotebook *notebook) {
     const char *tab_title = gtk_widget_get_tooltip_text(tab);
     tab_title = gtk_widget_get_tooltip_text(tab);
     gtk_notebook_append_page(notebook, tab, gtk_label_new(tab_title));
     gtk_widget_set_tooltip_text(tab, NULL);
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();

    gtk_builder_add_from_resource(builder, DEFAULT_UI_PATH(builder.ui), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_resource(provider, DEFAULT_UI_PATH(style.css));
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GObject *window = gtk_builder_get_object(builder, "window");
    GtkNotebook *notebook = GTK_NOTEBOOK(gtk_builder_get_object (builder, "notebook"));
    gtk_window_set_application(GTK_WINDOW(window), app);
    g_object_unref(builder);

    create_new_tab(tab1_new(), notebook);
    create_new_tab(tab2_new(), notebook);
    create_new_tab(tab3_new(), notebook);
    // create_new_tab(tab4_new(), notebook);

    GtkWidget* tab4 = tab4_new();
    const char *tab_title = gtk_widget_get_tooltip_text(tab4);
    tab_title = gtk_widget_get_tooltip_text(tab4);
    gtk_notebook_append_page(notebook, tab4, gtk_label_new(tab_title));
    gtk_widget_set_tooltip_text(tab4, NULL);
    g_signal_connect(window, "close-request", G_CALLBACK(tab4_on_main_window_close_requested), tab4);

    create_new_tab(tab5_new(), notebook);
    create_new_tab(tab6_new(), notebook);
    create_new_tab(tab7_new(), notebook);

    gtk_widget_set_visible(GTK_WIDGET(window), TRUE);
}

int main (int   argc, char *argv[]) {
#ifdef GTK_SRCDIR
    g_chdir (GTK_SRCDIR);
#endif

    GtkApplication *app = gtk_application_new ("org.gtkal.kakoyto", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
