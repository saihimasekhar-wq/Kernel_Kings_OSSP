#include "system_info.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

#define BUFFER_SIZE 4096

int get_system_info(SystemInfo *info)
{
    if (info == NULL)
    {
        return -1;
    }

    memset(info, 0, sizeof(SystemInfo));

    /*
     * Get hostname using gethostname().
     */
    if (gethostname(
            info->hostname,
            sizeof(info->hostname)) != 0)
    {
        return -1;
    }

    /*
     * Read kernel information from /proc/version.
     */
    char buffer[BUFFER_SIZE];

    int bytes_read =
        read_file("/proc/version", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    strncpy(
        info->kernel_version,
        buffer,
        sizeof(info->kernel_version) - 1
    );

    info->kernel_version[
        strcspn(info->kernel_version, "\n")
    ] = '\0';

    /*
     * Get system architecture using uname().
     */
    struct utsname system_info;

    if (uname(&system_info) != 0)
    {
        return -1;
    }

    strncpy(
        info->architecture,
        system_info.machine,
        sizeof(info->architecture) - 1
    );

    /*
     * Read system uptime from /proc/uptime.
     */
    bytes_read =
        read_file("/proc/uptime", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    if (sscanf(
            buffer,
            "%lf",
            &info->uptime_seconds) != 1)
    {
        return -1;
    }

    return 0;
}

void display_system_info(const SystemInfo *info)
{
    if (info == NULL)
    {
        return;
    }

    printf("\n");
    printf("SYSTEM INFORMATION\n");
    printf("----------------------------------------\n");

    printf(
        "Hostname        : %s\n",
        info->hostname
    );

    printf(
        "Kernel Version  : %s\n",
        info->kernel_version
    );

    printf(
        "Architecture    : %s\n",
        info->architecture
    );

    printf(
        "Uptime          : %.2f seconds\n",
        info->uptime_seconds
    );
}
