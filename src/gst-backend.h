#ifndef PROJECTGLIESE_GST_BACKEND_H
#define PROJECTGLIESE_GST_BACKEND_H

void backendInit(int *argc, char ***argv);
void backendDeInit();
int  backendSetWindow(gpointer wnd);
int  backendPlay(const gchar *filename);
void backendStop();
void backendResume();
void backendReset();

#endif //PROJECTGLIESE_GST_BACKEND_H
