#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellforge.h"

#define MAX_INPUT 1024
#define MAX_ARGS 64

int read_input(char *buffer, size_t size);
int tokenize(char *input, char *args[], int max_args);

void add_history_command(const char *command);
void show_history(void);
void free_history(void);

void execute_external(char *args[]);

void show_help(void);
int handle_builtin(char *args[], int argc);

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("\n");
    printf("============================================================\n");
    printf("                    FORGEOS SHELL\n");
    printf("       Linux System Information & Resource Monitor\n");
    printf("============================================================\n");
    printf("Type 'help' to see available commands.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1)
    {
        printf("forgeos$ ");
        fflush(stdout);

        if (!read_input(input, sizeof(input)))
        {
            printf("\n");
            break;
        }

        if (strlen(input) == 0)
            continue;

        add_history_command(input);

        int argc = tokenize(input, args, MAX_ARGS);

        if (argc == 0)
            continue;

        int result = handle_builtin(args, argc);

        if (result == 0)
            break;

        if (result == -1)
            execute_external(args);
    }

    free_history();

    printf("Goodbye!\n");

    return 0;
}
