#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM1 "/student1_marks"
#define READY1 "/student1_ready"
#define DONE1 "/student1_done"

#define SIZE (5 * sizeof(int))

int main()
{
    int fd;
    int *marks;
    int total = 0;
    float average;

    sem_t *ready;
    sem_t *done;

    printf("========== STUDENT 1 ==========\n");

    /* Open only Student 1 shared memory */
    fd = shm_open(SHM1, O_RDONLY, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    marks = mmap(NULL, SIZE,
                 PROT_READ,
                 MAP_SHARED, fd, 0);

    if (marks == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    /* Open synchronization semaphores */
    ready = sem_open(READY1, 0);
    done = sem_open(DONE1, 0);

    if (ready == SEM_FAILED || done == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    /* Wait until teacher writes marks */
    sem_wait(ready);

    printf("\nStudent 1\n");
    printf("--------------------------------\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Subject %d : %d\n", i + 1, marks[i]);
        total += marks[i];
    }

    average = total / 5.0;

    printf("\nTotal   : %d\n", total);
    printf("Average : %.2f\n", average);

    /* Tell teacher that Student 1 is finished */
    sem_post(done);

    munmap(marks, SIZE);
    close(fd);

    sem_close(ready);
    sem_close(done);

    return 0;
}
