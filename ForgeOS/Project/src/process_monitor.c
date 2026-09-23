#include "process_monitor.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

static int is_numeric(const char *name)
{
    if (name == NULL || *name == '\0')
    {
        return 0;
    }

    while (*name != '\0')
    {
        if (!isdigit((unsigned char)*name))
        {
            return 0;
        }

        name++;
    }

    return 1;
}

int get_process_list(ProcessInfo processes[], int max_processes)
{
    if (processes == NULL || max_processes <= 0)
    {
        return -1;
    }

    DIR *proc_dir = opendir("/proc");

    if (proc_dir == NULL)
    {
        return -1;
    }

    struct dirent *entry;
    int count = 0;

    while ((entry = readdir(proc_dir)) != NULL &&
           count < max_processes)
    {
        if (!is_numeric(entry->d_name))
        {
            continue;
        }

        int pid = atoi(entry->d_name);

        char status_path[BUFFER_SIZE];

        snprintf(
            status_path,
            sizeof(status_path),
            "/proc/%d/status",
            pid
        );

        char buffer[BUFFER_SIZE];

        int bytes_read =
            read_file(status_path, buffer, sizeof(buffer));

        if (bytes_read < 0)
        {
            continue;
        }

        processes[count].pid = pid;
        processes[count].name[0] = '\0';
        processes[count].state = '?';

        char *line = strtok(buffer, "\n");

        while (line != NULL)
        {
            if (strncmp(line, "Name:", 5) == 0)
            {
                sscanf(
                    line + 5,
                    " %255[^\n]",
                    processes[count].name
                );
            }
            else if (strncmp(line, "State:", 6) == 0)
            {
                sscanf(
                    line + 6,
                    " %c",
                    &processes[count].state
                );

                break;
            }

            line = strtok(NULL, "\n");
        }

        if (processes[count].name[0] != '\0')
        {
            count++;
        }
    }

    closedir(proc_dir);

    return count;
}

void display_process_list(ProcessInfo processes[], int count)
{
    if (processes == NULL || count <= 0)
    {
        printf("No processes found.\n");
        return;
    }

    printf("\n");
    printf("PROCESS MONITOR\n");
    printf("--------------------------------------------------------------\n");
    printf("%-8s %-35s %-8s\n", "PID", "NAME", "STATE");
    printf("--------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf(
            "%-8d %-35.35s %-8c\n",
            processes[i].pid,
            processes[i].name,
            processes[i].state
        );
    }

    printf("--------------------------------------------------------------\n");
    printf("Total processes displayed: %d\n", count);
}
