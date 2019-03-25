#include <gtk/gtk.h>
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


int createOpenMenu      (OpenMenu* openMenu,           GtkWidget* menubar);
int createVideoMenu     (VideoMenu* videoMenu,         GtkWidget* menubar);
int createAudioMenu     (AudioMenu* audioMenu,         GtkWidget* menubar);
int createSubtitlesMenu (SubtitlesMenu* subtitlesMenu, GtkWidget* menubar);
int createViewMenu      (ViewMenu* viewMenu,           GtkWidget* menubar);
int createOptionsMenu   (OptionsMenu* optionsMenu,     GtkWidget* menubar);
int createHelpMenu      (HelpMenu* helpMenu,           GtkWidget* menubar);
int createUi            (GtkWidget* window);
int createMenubar       (Menubar* menubar);

int createWindow(const char* name, int width, int height) {
    GtkWidget* window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_window_set_title(GTK_WINDOW(window), name);
    createUi(window);
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    return 0;
}

int createUi(GtkWidget* window) {
    GtkWidget* controls;
    GtkWidget* mainBox;
    GtkWidget* bottomPanel;
    GtkWidget* videoWindow;
    GtkWidget* slider;
    GtkWidget* playButton;
    GtkWidget* pauseButton;
    GtkWidget* stopButton;
    GtkWidget* volumeButton;
    GtkWidget* fullscreenButton;
    Menubar menubar;
    videoWindow = gtk_drawing_area_new();
    playButton  = gtk_button_new_with_label("▶");
    pauseButton = gtk_button_new_with_label("▮▮");
    stopButton  = gtk_button_new_with_label("■");
    volumeButton = gtk_volume_button_new();
    fullscreenButton = gtk_button_new_with_label("◱");
    slider = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    createMenubar(&menubar);

    controls = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start (GTK_BOX (controls), playButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), pauseButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (controls), stopButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX(controls), volumeButton, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX(controls), fullscreenButton, FALSE, FALSE, 2);

    bottomPanel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(bottomPanel), slider, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(bottomPanel), controls, FALSE, FALSE, 2);

    mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), menubar.menubar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(mainBox), videoWindow, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(mainBox), bottomPanel, FALSE, FALSE, 2);
    gtk_container_add (GTK_CONTAINER (window), mainBox);
    return 0;
}

int createMenubar(Menubar* menubar) {
    menubar->menubar = gtk_menu_bar_new();
    createOpenMenu(&menubar->openMenu, menubar->menubar);
    createVideoMenu(&menubar->videoMenu, menubar->menubar);
    createAudioMenu(&menubar->audioMenu, menubar->menubar);
    createSubtitlesMenu(&menubar->subtitlesMenu, menubar->menubar);
    createViewMenu(&menubar->viewMenu, menubar->menubar);
    createOptionsMenu(&menubar->optionsMenu, menubar->menubar);
    createHelpMenu(&menubar->helpMenu, menubar->menubar);
    return 0;
}

int createOpenMenu (OpenMenu* openMenu, GtkWidget* menubar) {
    openMenu->openMenu = gtk_menu_new();

    openMenu->OpenMi   =
            gtk_menu_item_new_with_label("Open");
    openMenu->fileMi   =
            gtk_menu_item_new_with_label("File");
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