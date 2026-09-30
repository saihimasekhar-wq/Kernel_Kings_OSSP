#include "cpu_monitor.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

/*
 * Read the total and idle CPU time from /proc/stat.
 */
static int read_cpu_times(
    unsigned long long *total,
    unsigned long long *idle)
{
    char buffer[1024];

    int bytes_read =
        read_file("/proc/stat", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    unsigned long long user = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle_time = 0;
    unsigned long long iowait = 0;
    unsigned long long irq = 0;
    unsigned long long softirq = 0;
    unsigned long long steal = 0;

    int result = sscanf(
        buffer,
        "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
        &user,
        &nice,
        &system,
        &idle_time,
        &iowait,
        &irq,
        &softirq,
        &steal
    );

    if (result < 4)
    {
        return -1;
    }

    /*
     * Idle time includes normal idle time and I/O wait time.
     */
    *idle = idle_time + iowait;

    *total =
        user +
        nice +
        system +
        idle_time +
        iowait +
        irq +
        softirq +
        steal;

    return 0;
}

/*
 * CPU usage is calculated using two snapshots
 * of /proc/stat separated by one second.
 */
static double calculate_cpu_usage(void)
{
    unsigned long long total1;
    unsigned long long idle1;

    unsigned long long total2;
    unsigned long long idle2;

    if (read_cpu_times(&total1, &idle1) != 0)
    {
        return -1.0;
    }

    sleep(1);

    if (read_cpu_times(&total2, &idle2) != 0)
    {
        return -1.0;
    }

    unsigned long long total_delta =
        total2 - total1;

    unsigned long long idle_delta =
        idle2 - idle1;

    if (total_delta == 0)
    {
        return 0.0;
    }

    double usage =
        100.0 *
        ((double)(total_delta - idle_delta) /
         (double)total_delta);

    /*
     * Keep the result within the valid range.
     */
    if (usage < 0.0)
    {
        usage = 0.0;
    }

    if (usage > 100.0)
    {
        usage = 100.0;
    }

    return usage;
}

int get_cpu_info(CPUInfo *info)
{
    if (info == NULL)
    {
        return -1;
    }

    memset(info, 0, sizeof(CPUInfo));

    char buffer[BUFFER_SIZE];

    /*
     * Read CPU model information from /proc/cpuinfo.
     */
    int bytes_read =
        read_file("/proc/cpuinfo", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    char *line = strtok(buffer, "\n");

    while (line != NULL)
    {
        if (strncmp(line, "model name", 10) == 0)
        {
            char *colon = strchr(line, ':');

            if (colon != NULL)
            {
                colon++;

                while (*colon == ' ')
                {
                    colon++;
                }

                strncpy(
                    info->model_name,
                    colon,
                    sizeof(info->model_name) - 1
                );

                break;
            }
        }

        line = strtok(NULL, "\n");
    }

    /*
     * Count logical processors from /proc/cpuinfo.
     */
/*
 * Get the number of online logical processors.
 */
long processor_count = sysconf(_SC_NPROCESSORS_ONLN);

if (processor_count < 1)
{
    return -1;
}

info->processor_count = (int)processor_count;

    /*
     * Calculate current CPU usage.
     */
    info->usage_percent = calculate_cpu_usage();

    if (info->usage_percent < 0.0)
    {
        return -1;
    }

    return 0;
}

void display_cpu_info(const CPUInfo *info)
{
    if (info == NULL)
    {
        return;
    }

    printf("\n");
    printf("CPU INFORMATION\n");
    printf("------------------------------\n");

    printf(
        "Model          : %s\n",
        info->model_name
    );

    printf(
        "Processors     : %d\n",
        info->processor_count
    );

    printf(
        "CPU Usage      : %.2f%%\n",
        info->usage_percent
    );
}
