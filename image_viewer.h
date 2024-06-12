// image_viewer.h

#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include <gtk/gtk.h>

extern void on_key_press(GtkWidget *widget, GdkEventKey *event);
extern void on_button_clicked(GtkButton *button, gpointer user_data);
extern gboolean on_button_enter(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);
extern gboolean on_button_leave(GtkWidget *widget, GdkEventCrossing *event, gpointer user_data);
void image_viewer_main(int argc, char *argv[]);

#endif // IMAGE_VIEWER_H

