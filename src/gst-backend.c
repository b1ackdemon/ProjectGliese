#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include "gst-backend.h"
#include "ui.h"

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

typedef struct _CustomData {
    GstState state;
    gint64 duration;
    GstStateChangeReturn ret;
} CustomData;

static GstElement* pipeline;
static CustomData data;

static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data);
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data);

void backendInit(int *argc, char ***argv){
    gst_init (argc, argv);
}

int backendSetWindow(guintptr window) {
    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(pipeline), window);
    return 0;
}

int backendPlay(const gchar *filename) {
    GstBus* bus;

    data.duration = GST_CLOCK_TIME_NONE;
    pipeline = gst_element_factory_make("playbin", "playbin");
    if (!pipeline) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }
    g_object_set (pipeline, "uri", "file:///home/blackdemon/Videos/3.mp4", NULL);

    bus = gst_element_get_bus (pipeline);
    gst_bus_add_signal_watch (bus);
    g_signal_connect (G_OBJECT (bus), "message::error", (GCallback)error_cb, &data);
    g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback)eos_cb, &data);
    g_signal_connect (G_OBJECT (bus), "message::state-changed", (GCallback)state_changed_cb, &data);
    gst_object_unref (bus);

    data.ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (data.ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return -1;
    }
    return 0;
}

gdouble backendQueryDuration() {
    gboolean result;

    result = gst_element_query_duration (pipeline, GST_FORMAT_TIME, &data.duration);
    if (!result) {
        g_printerr ("Could not query current duration.\n");
        return GST_CLOCK_TIME_NONE;
    }
    return  (gdouble) data.duration / GST_SECOND;
}

gboolean backendQueryPosition(gdouble* current) {
    gboolean res;
    gint64 cur;
    res = gst_element_query_position(pipeline, GST_FORMAT_TIME, &cur);
    *current = (gdouble) cur / GST_SECOND;
    return res;
}

gboolean backendDurationIsValid() {
    return GST_CLOCK_TIME_IS_VALID (data.duration);
}

gboolean backendIsPausedOrPlaying() {
    if (data.state < GST_STATE_PAUSED) {
        return FALSE;
    }
    return TRUE;
}

void backendStop() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_READY);
    }
}

void backendResume() {
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}

void backendPause() {
    gst_element_set_state(pipeline, GST_STATE_PAUSED);
}

void backendSeek(gdouble value) {
    gst_element_seek_simple(pipeline, GST_FORMAT_TIME,
            GST_SEEK_FLAG_FLUSH, (gint64)(value * GST_SECOND));
}

void backendSetVolume(gdouble volume) {
    g_object_set(pipeline, "volume", volume, NULL);
}

void backendDeInit() {
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
}

static void eos_cb (GstBus *bus, GstMessage *msg, CustomData *data) {
    g_print ("End-Of-Stream reached.\n");
    gst_element_set_state (pipeline, GST_STATE_READY);
}

/* This function is called when an error message is posted on the bus */
static void error_cb (GstBus *bus, GstMessage *msg, CustomData *data) {
    GError *err;
    gchar *debug_info;

    /* Print error details on the screen */
    gst_message_parse_error (msg, &err, &debug_info);
    g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (msg->src), err->message);
    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
    g_clear_error (&err);
    g_free (debug_info);

    /* Set the pipeline to READY (which stops playback) */
    gst_element_set_state (pipeline, GST_STATE_READY);
}

/* This function is called when the pipeline changes states. We use it to
 * keep track of the current state. */
static void state_changed_cb (GstBus *bus, GstMessage *msg, CustomData *data) {
    GstState old_state, new_state, pending_state;
    gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (pipeline)) {
        data->state = new_state;
        g_print ("State set to %s\n", gst_element_state_get_name (new_state));
        if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED) {
            /* For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state */
            refreshUi();
        }
    }
}