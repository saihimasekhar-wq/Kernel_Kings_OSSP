#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellforge.h"

int read_input(char *buffer, size_t size)
{
    if (fgets(buffer, size, stdin) == NULL)
    {
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    return 1;
}
