#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cpu {
    int pid;
    int at;
    int bt;
    int priority;
    int ct;
    int tat;
    int wt;
    int rt;
    int remaining;
    bool proc_status;
} Process;


/* ---------------- DISPLAY INPUT ---------------- */

void display(Process p[], int n) {
    printf("\nProcess Data:\n");
    printf("\nPID\tAT\tBT\tPRI\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].bt,
               p[i].priority);
    }
}


/* ---------------- DISPLAY RESULT ---------------- */

void scheduling_display(Process p[], int n) {
    float sum_tat = 0;
    float sum_wt = 0;

    printf("\nScheduling Data:\n");
    printf("\nPID\tAT\tBT\tPRI\tCT\tTAT\tWT\tRT\n");

    for (int i = 0; i < n; i++) {
        sum_tat += p[i].tat;
        sum_wt += p[i].wt;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].at,
               p[i].bt,
               p[i].priority,
               p[i].ct,
               p[i].tat,
               p[i].wt,
               p[i].rt);
    }

    printf("\nAverage TAT = %.2f\n", sum_tat / n);
    printf("Average WT  = %.2f\n", sum_wt / n);
    printf("Average RT  = %.2f\n", 
           (sum_tat - sum_wt) / n);
}


/* ---------------- RESET ---------------- */

void reset(Process p[], Process original[], int n) {
    for (int i = 0; i < n; i++) {
        p[i] = original[i];

        p[i].ct = 0;
        p[i].tat = 0;
        p[i].wt = 0;
        p[i].rt = -1;
        p[i].remaining = p[i].bt;
        p[i].proc_status = false;
    }
}


/* =================================================
                     FCFS
   ================================================= */

void FCFS(Process p[], int n) {

    // Sort according to Arrival Time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            if (p[j].at > p[j + 1].at) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int cpu_time = 0;

    for (int i = 0; i < n; i++) {

        if (cpu_time < p[i].at)
            cpu_time = p[i].at;

        p[i].rt = cpu_time - p[i].at;

        cpu_time += p[i].bt;

        p[i].ct = cpu_time;

        p[i].tat = p[i].ct - p[i].at;

        p[i].wt = p[i].tat - p[i].bt;
    }

    scheduling_display(p, n);
}


/* =================================================
                  SJF NON-PREEMPTIVE
   ================================================= */

void SJF(Process p[], int n) {

    int completed = 0;
    int cpu_time = 0;

    while (completed < n) {

        int index = -1;
        int shortest = 999999;

        // Find shortest arrived process
        for (int i = 0; i < n; i++) {

            if (!p[i].proc_status &&
                p[i].at <= cpu_time &&
                p[i].bt < shortest) {

                shortest = p[i].bt;
                index = i;
            }
        }

        // No process has arrived
        if (index == -1) {

            cpu_time++;

            continue;
        }

        // Response time
        p[index].rt = cpu_time - p[index].at;

        cpu_time += p[index].bt;

        p[index].ct = cpu_time;

        p[index].tat = p[index].ct - p[index].at;

        p[index].wt = p[index].tat - p[index].bt;

        p[index].proc_status = true;

        completed++;
    }

    scheduling_display(p, n);
}


/* =================================================
                      SRTF
   ================================================= */

void SRTF(Process p[], int n) {

    int completed = 0;
    int cpu_time = 0;

    while (completed < n) {

        int index = -1;
        int shortest = 999999;

        // Find process with shortest remaining time
        for (int i = 0; i < n; i++) {

            if (p[i].at <= cpu_time &&
                p[i].remaining > 0 &&
                p[i].remaining < shortest) {

                shortest = p[i].remaining;
                index = i;
            }
        }

        // CPU idle
        if (index == -1) {
            cpu_time++;
            continue;
        }

        // First time process gets CPU
        if (p[index].remaining == p[index].bt) {
            p[index].rt = cpu_time - p[index].at;
        }

        // Execute for 1 unit
        p[index].remaining--;
        cpu_time++;

        // Process completed
        if (p[index].remaining == 0) {

            p[index].ct = cpu_time;

            p[index].tat =
                p[index].ct - p[index].at;

            p[index].wt =
                p[index].tat - p[index].bt;

            p[index].proc_status = true;

            completed++;
        }
    }

    scheduling_display(p, n);
}


/* =================================================
             PRIORITY NON-PREEMPTIVE
   ================================================= */

void Priority_NonPreemptive(Process p[], int n) {

    int completed = 0;
    int cpu_time = 0;

    while (completed < n) {

        int index = -1;
        int best_priority = 999999;

        for (int i = 0; i < n; i++) {

            if (!p[i].proc_status &&
                p[i].at <= cpu_time &&
                p[i].priority < best_priority) {

                best_priority = p[i].priority;
                index = i;
            }
        }

        // CPU idle
        if (index == -1) {
            cpu_time++;
            continue;
        }

        p[index].rt = cpu_time - p[index].at;

        cpu_time += p[index].bt;

        p[index].ct = cpu_time;

        p[index].tat =
            p[index].ct - p[index].at;

        p[index].wt =
            p[index].tat - p[index].bt;

        p[index].proc_status = true;

        completed++;
    }

    scheduling_display(p, n);
}


/* =================================================
                PRIORITY PREEMPTIVE
   ================================================= */

void Priority_Preemptive(Process p[], int n) {

    int completed = 0;
    int cpu_time = 0;

    while (completed < n) {

        int index = -1;
        int best_priority = 999999;

        for (int i = 0; i < n; i++) {

            if (p[i].at <= cpu_time &&
                p[i].remaining > 0 &&
                p[i].priority < best_priority) {

                best_priority = p[i].priority;
                index = i;
            }
        }

        // CPU idle
        if (index == -1) {
            cpu_time++;
            continue;
        }

        // First execution
        if (p[index].remaining == p[index].bt) {
            p[index].rt = cpu_time - p[index].at;
        }

        // Execute 1 unit
        p[index].remaining--;
        cpu_time++;

        // Completed
        if (p[index].remaining == 0) {

            p[index].ct = cpu_time;

            p[index].tat =
                p[index].ct - p[index].at;

            p[index].wt =
                p[index].tat - p[index].bt;

            p[index].proc_status = true;

            completed++;
        }
    }

    scheduling_display(p, n);
}


/* =================================================
                  ROUND ROBIN
   ================================================= */

void RoundRobin(Process p[], int n, int quantum) {

    int completed = 0;
    int cpu_time = 0;

    int queue[100];
    int front = 0;
    int rear = 0;

    bool in_queue[10] = {false};

    while (completed < n) {

        // Add arrived processes
        for (int i = 0; i < n; i++) {

            if (p[i].at <= cpu_time &&
                p[i].remaining > 0 &&
                !in_queue[i]) {

                queue[rear++] = i;
                in_queue[i] = true;
            }
        }

        // CPU idle
        if (front == rear) {
            cpu_time++;
            continue;
        }

        int index = queue[front++];

        in_queue[index] = false;

        // First execution
        if (p[index].remaining == p[index].bt) {
            p[index].rt = cpu_time - p[index].at;
        }

        int execution_time;

        if (p[index].remaining < quantum)
            execution_time = p[index].remaining;
        else
            execution_time = quantum;

        // Execute
        for (int t = 0; t < execution_time; t++) {

            p[index].remaining--;
            cpu_time++;

            // Add newly arrived processes
            for (int i = 0; i < n; i++) {

                if (p[i].at <= cpu_time &&
                    p[i].remaining > 0 &&
                    !in_queue[i] &&
                    i != index) {

                    queue[rear++] = i;
                    in_queue[i] = true;
                }
            }

            if (p[index].remaining == 0)
                break;
        }

        // Process completed
        if (p[index].remaining == 0) {

            p[index].ct = cpu_time;

            p[index].tat =
                p[index].ct - p[index].at;

            p[index].wt =
                p[index].tat - p[index].bt;

            p[index].proc_status = true;

            completed++;
        }

        // Process not completed
        else {

            queue[rear++] = index;
            in_queue[index] = true;
        }
    }

    scheduling_display(p, n);
}


/* =================================================
                       MAIN
   ================================================= */

int main() {

    int n;
    int choice;
    int quantum;

    Process original[10];
    Process p[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Input
    for (int i = 0; i < n; i++) {

        original[i].pid = i + 1;

        printf("\nEnter AT of P%d: ", i + 1);
        scanf("%d", &original[i].at);

        printf("Enter BT of P%d: ", i + 1);
        scanf("%d", &original[i].bt);

        printf("Enter Priority of P%d: ", i + 1);
        scanf("%d", &original[i].priority);
    }

    display(original, n);

    while (1) {

        printf("\n====================================\n");
        printf("       CPU SCHEDULING ALGORITHMS\n");
        printf("====================================\n");

        printf("1. FCFS\n");
        printf("2. SJF\n");
        printf("3. SRTF\n");
        printf("4. Priority Non-Preemptive\n");
        printf("5. Priority Preemptive\n");
        printf("6. Round Robin\n");
        printf("7. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        // Reset data before every algorithm
        reset(p, original, n);

        switch (choice) {

            case 1:
                printf("\n--- FCFS ---\n");
                FCFS(p, n);
                break;

            case 2:
                printf("\n--- SJF ---\n");
                SJF(p, n);
                break;

            case 3:
                printf("\n--- SRTF ---\n");
                SRTF(p, n);
                break;

            case 4:
                printf("\n--- Priority Non-Preemptive ---\n");
                printf("Lower number = Higher priority\n");
                Priority_NonPreemptive(p, n);
                break;

            case 5:
                printf("\n--- Priority Preemptive ---\n");
                printf("Lower number = Higher priority\n");
                Priority_Preemptive(p, n);
                break;

            case 6:
                printf("\nEnter Time Quantum: ");
                scanf("%d", &quantum);

                printf("\n--- Round Robin ---\n");
                RoundRobin(p, n, quantum);
                break;

            case 7:
                printf("\nProgram exited.\n");
                exit(0);

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}
