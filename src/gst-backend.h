#pragma once

void backendInit(int *argc, char ***argv);
void backendDeInit();
int backendSetWindow(guintptr window);
int  backendPlay(const gchar *filename);
void backendPause();
void backendStop();
void backendResume();
void backendSeek(gdouble value);
gdouble backendQueryDuration();
gboolean backendQueryPosition(gdouble* current);
gboolean backendDurationIsValid();
gboolean backendIsPausedOrPlaying();