#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#define MAX_PROCESSES 100

typedef struct
{
    int pid;
    char name[256];
    char state;
} ProcessInfo;

int get_process_list(ProcessInfo processes[], int max_processes);

void display_process_list(ProcessInfo processes[], int count);

#endif
