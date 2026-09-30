#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shellforge.h"
#include "system_info.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "process_monitor.h"

void show_help(void)
{
    printf("\n");
    printf("========== FORGEOS SHELL COMMANDS ==========\n");
    printf("sysinfo      - Display system information\n");
    printf("cpu          - Display CPU information\n");
    printf("memory       - Display memory information\n");
    printf("disk         - Display disk I/O information\n");
    printf("processes    - Display running processes\n");
    printf("pwd          - Display current directory\n");
    printf("cd <dir>     - Change directory\n");
    printf("echo <text>  - Display text\n");
    printf("history      - Display command history\n");
    printf("help         - Display this help message\n");
    printf("clear        - Clear the terminal\n");
    printf("exit         - Exit ForgeOS Shell\n");
    printf("=============================================\n\n");
}

int handle_builtin(char *args[], int argc)
{
    if (argc == 0)
        return 1;

    if (strcmp(args[0], "help") == 0)
    {
        show_help();
        return 1;
    }

    if (strcmp(args[0], "sysinfo") == 0)
    {
        SystemInfo info;

        if (get_system_info(&info) == 0)
            display_system_info(&info);
        else
            printf("Error: Unable to retrieve system information.\n");

        return 1;
    }

    if (strcmp(args[0], "cpu") == 0)
    {
        CPUInfo info;

        if (get_cpu_info(&info) == 0)
            display_cpu_info(&info);
        else
            printf("Error: Unable to retrieve CPU information.\n");

        return 1;
    }

    if (strcmp(args[0], "memory") == 0)
    {
        MemoryInfo info;

        if (get_memory_info(&info) == 0)
            display_memory_info(&info);
        else
            printf("Error: Unable to retrieve memory information.\n");

        return 1;
    }

    if (strcmp(args[0], "disk") == 0)
    {
        DiskInfo info;

        if (get_disk_info(&info) == 0)
            display_disk_info(&info);
        else
            printf("Error: Unable to retrieve disk information.\n");

        return 1;
    }

    if (strcmp(args[0], "processes") == 0)
    {
        ProcessInfo processes[MAX_PROCESSES];

        int count = get_process_list(processes, MAX_PROCESSES);

        if (count >= 0)
            display_process_list(processes, count);
        else
            printf("Error: Unable to retrieve process information.\n");

        return 1;
    }

    if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[1024];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");

        return 1;
    }

    if (strcmp(args[0], "cd") == 0)
    {
        const char *path;

        if (argc < 2)
            path = getenv("HOME");
        else
            path = args[1];

        if (path == NULL || chdir(path) != 0)
            perror("cd");

        return 1;
    }

    if (strcmp(args[0], "history") == 0)
    {
        show_history();
        return 1;
    }

    if (strcmp(args[0], "clear") == 0)
    {
        printf("\033[2J\033[H");
        return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }

    if (strcmp(args[0], "echo") == 0)
    {
        for (int i = 1; i < argc; i++)
        {
            printf("%s", args[i]);

            if (i < argc - 1)
                printf(" ");
        }

        printf("\n");
        return 1;
    }

    return -1;
}
