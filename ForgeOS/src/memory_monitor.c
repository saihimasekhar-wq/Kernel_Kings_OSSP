#include "memory_monitor.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096

int get_memory_info(MemoryInfo *info)
{
    if (info == NULL)
    {
        return -1;
    }

    char buffer[BUFFER_SIZE];

    int bytes_read = read_file("/proc/meminfo", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    info->total_kb = 0;
    info->available_kb = 0;
    info->free_kb = 0;
    info->used_kb = 0;
    info->usage_percent = 0.0;

    char *line = strtok(buffer, "\n");

    while (line != NULL)
    {
        unsigned long value;

        if (sscanf(line, "MemTotal: %lu kB", &value) == 1)
        {
            info->total_kb = value;
        }
        else if (sscanf(line, "MemAvailable: %lu kB", &value) == 1)
        {
            info->available_kb = value;
        }
        else if (sscanf(line, "MemFree: %lu kB", &value) == 1)
        {
            info->free_kb = value;
        }

        line = strtok(NULL, "\n");
    }

    if (info->total_kb > 0)
    {
        info->used_kb = info->total_kb - info->available_kb;

        info->usage_percent =
            ((double)info->used_kb / info->total_kb) * 100.0;
    }

    return 0;
}

void display_memory_info(const MemoryInfo *info)
{
    if (info == NULL)
    {
        return;
    }

    printf("\n");
    printf("MEMORY INFORMATION\n");
    printf("------------------------------\n");

    printf("Total Memory     : %lu kB\n", info->total_kb);
    printf("Available Memory : %lu kB\n", info->available_kb);
    printf("Free Memory      : %lu kB\n", info->free_kb);
    printf("Used Memory      : %lu kB\n", info->used_kb);
    printf("Memory Usage     : %.2f%%\n", info->usage_percent);
}
