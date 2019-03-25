#include "gst_backend.h"
#include "ui.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    createWindow("ProjectGliese", 800, 600);
    gtk_main();
    return 0;
}