cmake_minimum_required(VERSION 3.13)
project(ProjectGliese C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/lib/libffi-3.2.1/include -I/usr/include/orc-0.4 -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/uuid -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/gio-unix-2.0 -I/usr/include/libdrm -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -pthread -lgstvideo-1.0 -lgstbase-1.0 -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0")
add_executable(ProjectGliese ui.c gst-backend.c gst-backend.h)