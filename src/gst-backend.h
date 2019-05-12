#pragma once

void backendInit (int* argc, char*** argv);
void backendDeInit();
int backendSetWindow (guintptr window);
int  backendPlay (const gchar *filename);
void backendPause();
void backendStop();
void backendResume();
void backendSeek (gdouble value);
void backendSetVolume (gdouble volume);
void backendGetInformationAboutStreams(GtkTextBuffer *textBuffer);
void backendGetDuration (gchar* str);
void backendGetPosition (gchar* str);
gdouble backendQueryDuration();
gdouble backendGetVolume();
gboolean backendQueryPosition (gdouble* current);
gboolean backendDurationIsValid();
gboolean backendIsPausedOrPlaying();
gboolean backendIsPlaying();