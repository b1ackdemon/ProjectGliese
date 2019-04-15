#include <gtk/gtk.h>

#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

#include "gst-backend.h"
#include "ui.h"

typedef struct _OpenMenu {
    GtkWidget* openMenu;
    GtkWidget* OpenMi;
    GtkWidget* fileMi;
    GtkWidget* closeMi;
    GtkWidget* exitMi;
} OpenMenu;

typedef struct _VideoMenu {
    GtkWidget* videoMenu;
    GtkWidget* videoMi;
    GtkWidget* trackMi;
    GtkWidget* fullscreenMi;
} VideoMenu;

typedef struct _AudioMenu {
    GtkWidget* audioMenu;
    GtkWidget* audioMi;
    GtkWidget* trackMi;
    GtkWidget* muteMi;
} AudioMenu;

typedef struct _SubtitlesMenu {
    GtkWidget* subtitlesMenu;
    GtkWidget* subtitlesMi;
    GtkWidget* primaryTrackMi;
    GtkWidget* secondaryTrackMi;
} SubtitlesMenu;

typedef struct _ViewMenu {
    GtkWidget* viewMenu;
    GtkWidget* viewMi;
    GtkWidget* informationAndPropertiesMi;
} ViewMenu;

typedef struct _OptionsMenu {
    GtkWidget* optionsMenu;
    GtkWidget* optionsMi;
    GtkWidget* preferencesMi;
} OptionsMenu;

typedef struct _HelpMenu {
    GtkWidget* helpMenu;
    GtkWidget* helpMi;
    GtkWidget* aboutMi;
} HelpMenu;

typedef struct _Menubar {
    GtkWidget*    menubar;
    OpenMenu      openMenu;
    VideoMenu     videoMenu;
    AudioMenu     audioMenu;
    SubtitlesMenu subtitlesMenu;
    ViewMenu      viewMenu;
    OptionsMenu   optionsMenu;
    HelpMenu      helpMenu;
} Menubar;

typedef struct _UiWidgets {
    GtkWidget* videoWindow;
    GtkWidget* playButton;
    GtkWidget* pauseButton;
    GtkWidget* stopButton;
    GtkWidget* volumeButton;
    GtkWidget* fullscreenButton;
} UiWidgets;

static GtkWidget* slider;
static UiWidgets uiWidgets;
static gulong sliderUpdateSignalId;

int createOpenMenu      (OpenMenu* openMenu, GtkWidget* menubar, UiWidgets *uiWidgets);
int createVideoMenu     (VideoMenu* videoMenu,         GtkWidget* menubar);
int createAudioMenu     (AudioMenu* audioMenu,         GtkWidget* menubar);
int createSubtitlesMenu (SubtitlesMenu* subtitlesMenu, GtkWidget* menubar);
int createViewMenu      (ViewMenu* viewMenu,           GtkWidget* menubar);
int createOptionsMenu   (OptionsMenu* optionsMenu,     GtkWidget* menubar);
int createHelpMenu      (HelpMenu* helpMenu,           GtkWidget* menubar);
int createUi            (GtkWidget* window);
int createMenubar       (Menubar* menubar, UiWidgets *uiWidgets);
int createWindow(const char* name, int width, int height);
static void createContext (GtkWidget *widget);

static void play_cb (GtkButton *button, gpointer data);
static void pause_cb (GtkButton *button, gpointer data);
static void stop_cb (GtkButton *button, gpointer data);
static void slider_cb (GtkRange *range, gpointer data);
static void volume_cb (GtkRange* volumeButton, gpointer data);
static void deleteEvent_cb(GtkWidget *widget, GdkEvent *event, gpointer data);
static void fileMenu_cb (GtkWidget *widget, UiWidgets *uiWidgets);

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    backendInit(&argc, &argv);


    createWindow("ProjectGliese", 800, 520);

    g_timeout_add_seconds(1, (GSourceFunc) refreshUi, NULL);

    /* Start the GTK mail loop. */
    gtk_main();

    backendDeInit();
    return 0;
}
int createWindow(const char* name, int width, int height) {
    GtkWidget* window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_title(GTK_WINDOW(window), name);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK(deleteEvent_cb), NULL);
    createUi(window);
    gtk_widget_show_all(window);
    return 0;
}

int createUi(GtkWidget* window) {
    GtkWidget* controls;
    GtkWidget* mainBox;
    GtkWidget* bottomPanel;

    Menubar menubar;

    uiWidgets.videoWindow  = gtk_drawing_area_new();
    uiWidgets.playButton   = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON);
    uiWidgets.pauseButton  = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON);
    uiWidgets.stopButton   = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON);
    uiWidgets.volumeButton = gtk_volume_button_new();
    gtk_scale_button_set_value(GTK_SCALE_BUTTON(uiWidgets.volumeButton), 1.0);



    uiWidgets.fullscreenButton = gtk_button_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON);

    slider = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_scale_set_draw_value(GTK_SCALE(slider), 0);


    createMenubar(&menubar, &uiWidgets);

    controls = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (controls), uiWidgets.playButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), uiWidgets.pauseButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), uiWidgets.stopButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX(controls), uiWidgets.volumeButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX(controls), uiWidgets.fullscreenButton, FALSE, FALSE, 2);

    bottomPanel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width(GTK_CONTAINER(bottomPanel), 5);
    gtk_box_pack_start(GTK_BOX(bottomPanel), slider, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(bottomPanel), controls, FALSE, FALSE, 2);

    mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), menubar.menubar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), uiWidgets.videoWindow, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), bottomPanel, FALSE, FALSE, 2);
    gtk_container_add (GTK_CONTAINER (window), mainBox);
    return 0;
}

gboolean refreshUi() {
    gdouble current  = -1;
    gdouble duration = 0;

    if (!backendIsPausedOrPlaying()) {
        return TRUE;
    }

    if (!backendDurationIsValid()) {
        duration = backendQueryDuration();
        gtk_range_set_range(GTK_RANGE(slider), 0, duration);
    }

    if (backendQueryPosition(&current)) {
        /* Block the "value-changed" signal, so the slider_cb function is not called
         * (which would trigger a seek the user has not requested) */
        g_signal_handler_block (slider, sliderUpdateSignalId);
        /* Set the position of the slider to the current pipeline position, in SECONDS */
        gtk_range_set_value (GTK_RANGE (slider), current);
        /* Re-enable the signal */
        g_signal_handler_unblock (slider, sliderUpdateSignalId);
    }
    return TRUE;
}

int createMenubar(Menubar* menubar, UiWidgets *uiWidgets) {
    menubar->menubar = gtk_menu_bar_new();
    createOpenMenu(&menubar->openMenu, menubar->menubar, uiWidgets);
    createVideoMenu(&menubar->videoMenu, menubar->menubar);
    createAudioMenu(&menubar->audioMenu, menubar->menubar);
    createSubtitlesMenu(&menubar->subtitlesMenu, menubar->menubar);
    createViewMenu(&menubar->viewMenu, menubar->menubar);
    createOptionsMenu(&menubar->optionsMenu, menubar->menubar);
    createHelpMenu(&menubar->helpMenu, menubar->menubar);
    return 0;
}

int createOpenMenu (OpenMenu* openMenu, GtkWidget* menubar, UiWidgets *uiWidgets) {
    openMenu->openMenu = gtk_menu_new();

    openMenu->OpenMi   =
            gtk_menu_item_new_with_label("Open");
    openMenu->fileMi   =
            gtk_menu_item_new_with_label("File");
    g_signal_connect (openMenu->fileMi, "activate", G_CALLBACK (fileMenu_cb), uiWidgets);
    openMenu->closeMi  =
            gtk_menu_item_new_with_label("Close");
    openMenu->exitMi   =
            gtk_menu_item_new_with_label("Exit");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(openMenu->OpenMi),
            openMenu->openMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(openMenu->openMenu),
            openMenu->fileMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(openMenu->openMenu),
            openMenu->closeMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(openMenu->openMenu), openMenu->exitMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), openMenu->OpenMi);
    return 0;
}

int createVideoMenu (VideoMenu* videoMenu, GtkWidget* menubar) {
    videoMenu->videoMenu    = gtk_menu_new();

    videoMenu->videoMi      =
            gtk_menu_item_new_with_label("Video");
    videoMenu->trackMi      =
            gtk_menu_item_new_with_label("Track");
    videoMenu->fullscreenMi =
            gtk_menu_item_new_with_label("FullScreen");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(videoMenu->videoMi),
            videoMenu->videoMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(videoMenu->videoMenu),
            videoMenu->trackMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(videoMenu->videoMenu),
            videoMenu->fullscreenMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), videoMenu->videoMi);
    return 0;
}

int createAudioMenu (AudioMenu* audioMenu, GtkWidget* menubar) {
    audioMenu->audioMenu = gtk_menu_new();

    audioMenu->audioMi   =
            gtk_menu_item_new_with_label("Audio");
    audioMenu->trackMi   =
            gtk_menu_item_new_with_label("Track");
    audioMenu->muteMi    =
            gtk_menu_item_new_with_label("Mute");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(audioMenu->audioMi),
            audioMenu->audioMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(audioMenu->audioMenu),
            audioMenu->trackMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(audioMenu->audioMenu),
            audioMenu->muteMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),
            audioMenu->audioMi);
    return 0;
}

int createSubtitlesMenu (SubtitlesMenu* subtitlesMenu, GtkWidget* menubar) {
    subtitlesMenu->subtitlesMenu    = gtk_menu_new();

    subtitlesMenu->subtitlesMi      =
            gtk_menu_item_new_with_label("Subtitles");
    subtitlesMenu->primaryTrackMi   =
            gtk_menu_item_new_with_label("Primary track");
    subtitlesMenu->secondaryTrackMi =
            gtk_menu_item_new_with_label("Secondary track");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(subtitlesMenu->subtitlesMi),
            subtitlesMenu->subtitlesMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(subtitlesMenu->subtitlesMenu),
            subtitlesMenu->primaryTrackMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(subtitlesMenu->subtitlesMenu),
            subtitlesMenu->secondaryTrackMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),
            subtitlesMenu->subtitlesMi);
    return 0;
}

int createViewMenu (ViewMenu* viewMenu, GtkWidget* menubar) {
    viewMenu->viewMenu = gtk_menu_new();

    viewMenu->viewMi   = gtk_menu_item_new_with_label("View");
    viewMenu->informationAndPropertiesMi =
            gtk_menu_item_new_with_label("Information and properties");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(viewMenu->viewMi),
            viewMenu->viewMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewMenu->viewMenu),
            viewMenu->informationAndPropertiesMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), viewMenu->viewMi);
    return 0;
}

int createOptionsMenu (OptionsMenu* optionsMenu, GtkWidget* menubar) {
    optionsMenu->optionsMenu   = gtk_menu_new();

    optionsMenu->optionsMi     =
            gtk_menu_item_new_with_label("Options");
    optionsMenu->preferencesMi =
            gtk_menu_item_new_with_label("Preferences");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(optionsMenu->optionsMi),
            optionsMenu->optionsMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu->optionsMenu),
            optionsMenu->preferencesMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar),
            optionsMenu->optionsMi);
    return 0;
}

int createHelpMenu (HelpMenu* helpMenu, GtkWidget* menubar) {
    helpMenu->helpMenu = gtk_menu_new();

    helpMenu->helpMi  = gtk_menu_item_new_with_label("Help");
    helpMenu->aboutMi = gtk_menu_item_new_with_label("About");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMenu->helpMi),
            helpMenu->helpMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu->helpMenu),
            helpMenu->aboutMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMenu->helpMi);
    return 0;
}

static void createContext (GtkWidget *widget) {
    GdkWindow *window = gtk_widget_get_window (widget);
    guintptr window_handle;

    if (!gdk_window_ensure_native (window))
        g_error ("Couldn't create native window needed for GstVideoOverlay!");

#if defined (GDK_WINDOWING_WIN32)
    window_handle = (guintptr)GDK_WINDOW_HWND (window);
#elif defined (GDK_WINDOWING_QUARTZ)
    window_handle = gdk_quartz_window_get_nsview (window);
#elif defined (GDK_WINDOWING_X11)
    window_handle = GDK_WINDOW_XID (window);
#endif
    backendSetWindow(window_handle);
}

static void play_cb (GtkButton *button, gpointer data) {
    backendResume();
}

static void pause_cb (GtkButton *button, gpointer data) {
    backendPause();
}

static void stop_cb (GtkButton *button, gpointer data) {
    backendStop();
}

static void slider_cb (GtkRange *range, gpointer data) {
    gdouble value = gtk_range_get_value(GTK_RANGE(range));
    backendSeek(value);
}

static void volume_cb (GtkRange* volumeButton, gpointer data) {
    gdouble value = gtk_scale_button_get_value(GTK_SCALE_BUTTON(volumeButton));
    backendSetVolume(value);
}

void fileMenu_cb (GtkWidget *widget, UiWidgets *uiWidgets1) {
    GtkFileChooserNative *fileChooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkWindow *window = GTK_WINDOW(gtk_widget_get_toplevel(widget));
    int res;

    fileChooser = gtk_file_chooser_native_new("Open File", window,
            action, "_Open", "_Cancel");

    res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(fileChooser));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(fileChooser);
        filename = gtk_file_chooser_get_filename(chooser);
        const char *path = g_strconcat("file://", filename, NULL);

        backendPlay(path);
        createContext(uiWidgets.videoWindow);

        g_signal_connect (G_OBJECT (uiWidgets.playButton), "clicked", G_CALLBACK (play_cb), NULL);
        g_signal_connect (G_OBJECT (uiWidgets.pauseButton), "clicked", G_CALLBACK (pause_cb), NULL);
        g_signal_connect (G_OBJECT (uiWidgets.stopButton), "clicked", G_CALLBACK (stop_cb), NULL);
        g_signal_connect (G_OBJECT(uiWidgets.volumeButton), "value-changed", G_CALLBACK(volume_cb), NULL);
        sliderUpdateSignalId = g_signal_connect (G_OBJECT (slider), "value-changed", G_CALLBACK (slider_cb), NULL);
        gtk_scale_button_set_value(GTK_SCALE_BUTTON(uiWidgets.volumeButton), 1.0);

        g_free(filename);
    }
    g_object_unref(fileChooser);
}

/* This function is called when the main window is closed */
static void deleteEvent_cb(GtkWidget *widget, GdkEvent *event, gpointer data) {
    stop_cb (NULL, NULL);
    gtk_main_quit ();
}