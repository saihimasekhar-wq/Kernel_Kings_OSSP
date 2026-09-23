#include "disk_monitor.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 8192

/*
 * Get the device name of the filesystem mounted at /.
 *
 * Example:
 * /dev/sdd  ->  sdd
 */
static int get_root_device(char *device_name, size_t size)
{
    if (device_name == NULL || size == 0)
    {
        return -1;
    }

    FILE *mounts = fopen("/proc/mounts", "r");

    if (mounts == NULL)
    {
        return -1;
    }

    char device[256];
    char mount_point[256];
    char filesystem[64];
    char options[512];

    while (fscanf(
               mounts,
               "%255s %255s %63s %511s %*d %*d",
               device,
               mount_point,
               filesystem,
               options) == 4)
    {
        if (strcmp(mount_point, "/") == 0)
        {
            const char *name = device;

            if (strncmp(device, "/dev/", 5) == 0)
            {
                name = device + 5;
            }

            strncpy(device_name, name, size - 1);
            device_name[size - 1] = '\0';

            fclose(mounts);

            return 0;
        }
    }

    fclose(mounts);

    return -1;
}

int get_disk_info(DiskInfo *info)
{
    if (info == NULL)
    {
        return -1;
    }

    memset(info, 0, sizeof(DiskInfo));

    /*
     * Automatically detect the device containing /.
     */
    if (get_root_device(
            info->device_name,
            sizeof(info->device_name)) != 0)
    {
        return -1;
    }

    char buffer[BUFFER_SIZE];

    /*
     * Read Linux disk statistics.
     */
    int bytes_read =
        read_file("/proc/diskstats", buffer, sizeof(buffer));

    if (bytes_read < 0)
    {
        return -1;
    }

    char *line = strtok(buffer, "\n");

    while (line != NULL)
    {
        unsigned int major;
        unsigned int minor;

        char device[32];

        unsigned long long reads_completed;
        unsigned long long reads_merged;
        unsigned long long sectors_read;
        unsigned long long read_time;

        unsigned long long writes_completed;
        unsigned long long writes_merged;
        unsigned long long sectors_written;
        unsigned long long write_time;

        int result = sscanf(
            line,
            "%u %u %31s %llu %llu %llu %llu "
            "%llu %llu %llu %llu",
            &major,
            &minor,
            device,
            &reads_completed,
            &reads_merged,
            &sectors_read,
            &read_time,
            &writes_completed,
            &writes_merged,
            &sectors_written,
            &write_time
        );

        if (result == 11 &&
            strcmp(device, info->device_name) == 0)
        {
            info->reads_completed = reads_completed;
            info->sectors_read = sectors_read;

            info->writes_completed = writes_completed;
            info->sectors_written = sectors_written;

            /*
             * Linux disk statistics report sectors.
             * A sector is normally 512 bytes.
             */
            info->read_mb =
                (double)(sectors_read * 512ULL) /
                (1024.0 * 1024.0);

            info->write_mb =
                (double)(sectors_written * 512ULL) /
                (1024.0 * 1024.0);

            return 0;
        }

        line = strtok(NULL, "\n");
    }

    return -1;
}

void display_disk_info(const DiskInfo *info)
{
    if (info == NULL)
    {
        return;
    }

    printf("\n");
    printf("DISK INFORMATION\n");
    printf("------------------------------\n");

    printf(
        "Device           : %s\n",
        info->device_name
    );

    printf(
        "Reads Completed  : %llu\n",
        info->reads_completed
    );

    printf(
        "Sectors Read     : %llu\n",
        info->sectors_read
    );

    printf(
        "Data Read        : %.2f MB\n",
        info->read_mb
    );

    printf(
        "Writes Completed : %llu\n",
        info->writes_completed
    );

    printf(
        "Sectors Written  : %llu\n",
        info->sectors_written
    );

    printf(
        "Data Written     : %.2f MB\n",
        info->write_mb
    );
}
