#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

typedef struct
{
    char device_name[32];

    unsigned long long reads_completed;
    unsigned long long sectors_read;

    unsigned long long writes_completed;
    unsigned long long sectors_written;

    double read_mb;
    double write_mb;
} DiskInfo;

int get_disk_info(DiskInfo *info);

void display_disk_info(const DiskInfo *info);

#endif
