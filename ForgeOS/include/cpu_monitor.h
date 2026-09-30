#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

typedef struct
{
    char model_name[256];
    int processor_count;
    double usage_percent;
} CPUInfo;

int get_cpu_info(CPUInfo *info);

void display_cpu_info(const CPUInfo *info);

#endif
