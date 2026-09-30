#ifndef SHELLFORGE_H
#define SHELLFORGE_H

#include <stddef.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 50

void shell_loop(void);

int read_input(char *buffer, size_t size);
int tokenize(char *input, char *args[], int max_args);

void add_history_command(const char *command);
void show_history(void);
void free_history(void);

void execute_external(char *args[]);

void show_help(void);
int handle_builtin(char *args[], int argc);

#endif
