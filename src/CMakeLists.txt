cmake_minimum_required(VERSION 3.13)
project(ProjectGliese C)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig)

pkg_check_modules(GST REQUIRED
        gstreamer-1.0>=1.4
        gstreamer-video-1.0>=1.4)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(ProjectGliese ui.c gst-backend.c gst-backend.h ui.h)

target_link_libraries(ProjectGliese ${GST_LIBRARIES} ${GTK3_LIBRARIES})
target_include_directories(ProjectGliese PUBLIC ${GST_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
target_compile_options(ProjectGliese PUBLIC ${GST_CFLAGS} ${GTK3_CFLAGS})
