#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

typedef struct
{
    char hostname[256];
    char kernel_version[256];
    char architecture[128];
    double uptime_seconds;
} SystemInfo;

int get_system_info(SystemInfo *info);

void display_system_info(const SystemInfo *info);

#endif
