#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

typedef struct
{
    unsigned long total_kb;
    unsigned long available_kb;
    unsigned long free_kb;
    unsigned long used_kb;
    double usage_percent;
} MemoryInfo;

int get_memory_info(MemoryInfo *info);

void display_memory_info(const MemoryInfo *info);

#endif
