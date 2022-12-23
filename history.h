#ifndef HISTORY_H
#define HISTORY_H

int read_history(char **cmd, char *root);
void add_history(char *ip, char *root);
void show_history(char *limitstr, char *root);

#endif