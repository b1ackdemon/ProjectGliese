#ifndef PROJECTGLIESE_GST_BACKEND_H
#define PROJECTGLIESE_GST_BACKEND_H

void backendInit(int *argc, char ***argv);
void backendDeInit();
int backendSetWindow(guintptr window);
int  backendPlay(const gchar *filename);
void backendPause();
void backendStop();
void backendResume();
void backendSeek(gdouble value);

#endif //PROJECTGLIESE_GST_BACKEND_H
