#include <gtk/gtk.h>

#include "main_window.h"

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget* window = main_window_new(app);
    gtk_window_present(GTK_WINDOW(window));
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
