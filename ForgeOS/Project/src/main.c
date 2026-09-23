#include <stdio.h>
#include <unistd.h>

#include "system_info.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "process_monitor.h"

#define MAX_PROCESSES 100
#define REFRESH_INTERVAL 10

int main(void)
{
    while (1)
    {
        SystemInfo system;
        CPUInfo cpu;
        MemoryInfo memory;
        DiskInfo disk;
        ProcessInfo processes[MAX_PROCESSES];

        /*
         * Clear the terminal before every refresh.
         */
        printf("\033[2J");
        printf("\033[H");

        printf("============================================================\n");
        printf("       LINUX SYSTEM INFORMATION & RESOURCE MONITOR\n");
        printf("============================================================\n");

        /*
         * System information
         */
        if (get_system_info(&system) == 0)
        {
            display_system_info(&system);
        }
        else
        {
            printf("\nFailed to obtain system information.\n");
        }

        /*
         * CPU information
         */
        if (get_cpu_info(&cpu) == 0)
        {
            display_cpu_info(&cpu);
        }
        else
        {
            printf("\nFailed to obtain CPU information.\n");
        }

        /*
         * Memory information
         */
        if (get_memory_info(&memory) == 0)
        {
            display_memory_info(&memory);
        }
        else
        {
            printf("\nFailed to obtain memory information.\n");
        }

        /*
         * Disk information
         */
        if (get_disk_info(&disk) == 0)
        {
            display_disk_info(&disk);
        }
        else
        {
            printf("\nFailed to obtain disk information.\n");
        }

        /*
         * Process information
         */
        int process_count =
            get_process_list(
                processes,
                MAX_PROCESSES
            );

        if (process_count >= 0)
        {
            display_process_list(
                processes,
                process_count
            );
        }
        else
        {
            printf("\nFailed to obtain process information.\n");
        }

        printf("\n============================================================\n");
        printf("Refreshing every %d seconds | Press Ctrl+C to exit\n",
               REFRESH_INTERVAL);
        printf("============================================================\n");

        sleep(REFRESH_INTERVAL);
    }

    return 0;
}
