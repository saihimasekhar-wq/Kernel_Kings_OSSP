#include <stdio.h>
#include <string.h>
#include "shellforge.h"

int tokenize(char *input, char *args[], int max_args)
{
    int count = 0;
    char *token;
    char *saveptr = NULL;

    token = strtok_r(input, " \t", &saveptr);

    while (token != NULL && count < max_args - 1)
    {
        args[count++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }

    args[count] = NULL;

    return count;
}
