#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellforge.h"

static char *history[HISTORY_SIZE];
static int history_count = 0;

void add_history_command(const char *command)
{
    if (command == NULL || strlen(command) == 0)
        return;

    if (history_count == HISTORY_SIZE)
    {
        free(history[0]);

        for (int i = 1; i < HISTORY_SIZE; i++)
        {
            history[i - 1] = history[i];
        }

        history_count--;
    }

    history[history_count] = strdup(command);

    if (history[history_count] != NULL)
        history_count++;
}

void show_history(void)
{
    for (int i = 0; i < history_count; i++)
    {
        printf("%d  %s\n", i + 1, history[i]);
    }
}

void free_history(void)
{
    for (int i = 0; i < history_count; i++)
    {
        free(history[i]);
    }
}
