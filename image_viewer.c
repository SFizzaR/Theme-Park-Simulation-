#include "image_viewer.h"

void on_key_press(GtkWidget *widget, GdkEventKey *event) {
    if (event->keyval == GDK_KEY_F11) {
        gtk_window_fullscreen(GTK_WINDOW(widget));
    }
}

void on_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = GTK_WIDGET(user_data);
    gtk_widget_destroy(window);
}

gboolean on_button_enter(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    GtkWidget *image_widget = GTK_WIDGET(user_data);
    gtk_image_set_from_file(GTK_IMAGE(image_widget), "/home/fizza/Desktop/OsProject/Intro_Hover.png");
    return FALSE;
}

gboolean on_button_leave(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data) {
    GtkWidget *image_widget = GTK_WIDGET(user_data);
    gtk_image_set_from_file(GTK_IMAGE(image_widget), "/home/fizza/Desktop/OsProject/Intro.png");
    return FALSE;
}

typedef struct {
    GtkWidget *image_widget;
    const char *image_path;
} ImageLoadData;

gboolean set_image(gpointer data) {
    ImageLoadData *image_data = (ImageLoadData *)data;
    gtk_image_set_from_file(GTK_IMAGE(image_data->image_widget), image_data->image_path);
    g_free(image_data);
    return FALSE;
}

void *load_image_async(void *data) {
    ImageLoadData *image_data = (ImageLoadData *)data;
    gdk_threads_add_idle(set_image, image_data);
    return NULL;
}

void image_viewer_main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *overlay;
    GtkWidget *fixed;
    GtkWidget *image_widget;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 1006, 700); // Set initial window size

    overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window), overlay);

    fixed = gtk_fixed_new();
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), fixed);

    button = gtk_button_new_with_label("Close");
    gtk_widget_set_size_request(button, 273, 70); // Set the button size to 273x70 pixels
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
    
    // Set the button's position within the fixed container
    gtk_fixed_put(GTK_FIXED(fixed), button, 710, 70); // x=710, y=70

    image_widget = gtk_image_new();
    gtk_widget_set_halign(image_widget, GTK_ALIGN_FILL);
    gtk_widget_set_valign(image_widget, GTK_ALIGN_FILL);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), image_widget);

    // Make the image pass events to the button behind it
    gtk_overlay_set_overlay_pass_through(GTK_OVERLAY(overlay), image_widget, TRUE);

    // Connect hover signals to button
    g_signal_connect(button, "enter-notify-event", G_CALLBACK(on_button_enter), image_widget);
    g_signal_connect(button, "leave-notify-event", G_CALLBACK(on_button_leave), image_widget);

    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);

    // Set the initial image synchronously
    gtk_image_set_from_file(GTK_IMAGE(image_widget), "/home/fizza/Desktop/OsProject/Intro.png");

    gtk_main();
}

