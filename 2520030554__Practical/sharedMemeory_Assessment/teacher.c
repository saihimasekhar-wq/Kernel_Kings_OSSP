#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>

#define SHM1 "/student1_marks"
#define SHM2 "/student2_marks"

#define READY1 "/student1_ready"
#define READY2 "/student2_ready"

#define DONE1 "/student1_done"
#define DONE2 "/student2_done"

#define SIZE (5 * sizeof(int))

int main()
{
    int fd1, fd2;
    int *marks1, *marks2;

    sem_t *ready1, *ready2;
    sem_t *done1, *done2;

    printf("========== TEACHER (PRODUCER) ==========\n");

    /* Remove old shared memory/semaphores if they exist */
    shm_unlink(SHM1);
    shm_unlink(SHM2);

    sem_unlink(READY1);
    sem_unlink(READY2);
    sem_unlink(DONE1);
    sem_unlink(DONE2);

    /* Create Student 1 shared memory */
    fd1 = shm_open(SHM1, O_CREAT | O_RDWR, 0666);

    if (fd1 == -1)
    {
        perror("shm_open student1");
        exit(1);
    }

    ftruncate(fd1, SIZE);

    marks1 = mmap(NULL, SIZE,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd1, 0);

    if (marks1 == MAP_FAILED)
    {
        perror("mmap student1");
        exit(1);
    }

    /* Create Student 2 shared memory */
    fd2 = shm_open(SHM2, O_CREAT | O_RDWR, 0666);

    if (fd2 == -1)
    {
        perror("shm_open student2");
        exit(1);
    }

    ftruncate(fd2, SIZE);

    marks2 = mmap(NULL, SIZE,
                  PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd2, 0);

    if (marks2 == MAP_FAILED)
    {
        perror("mmap student2");
        exit(1);
    }

    /* Create READY semaphores */
    ready1 = sem_open(READY1, O_CREAT, 0666, 0);
    ready2 = sem_open(READY2, O_CREAT, 0666, 0);

    /* Create DONE semaphores */
    done1 = sem_open(DONE1, O_CREAT, 0666, 0);
    done2 = sem_open(DONE2, O_CREAT, 0666, 0);

    if (ready1 == SEM_FAILED || ready2 == SEM_FAILED ||
        done1 == SEM_FAILED || done2 == SEM_FAILED)
    {
        perror("sem_open");
        exit(1);
    }

    /* Enter Student 1 marks */
    printf("\nEnter marks for Student 1:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Subject %d: ", i + 1);
        scanf("%d", &marks1[i]);
    }

    /* Enter Student 2 marks */
    printf("\nEnter marks for Student 2:\n");

    for (int i = 0; i < 5; i++)
    {
        printf("Subject %d: ", i + 1);
        scanf("%d", &marks2[i]);
    }

    printf("\nMarks successfully written to shared memory.\n");

    /* Notify students */
    sem_post(ready1);
    sem_post(ready2);

    printf("Students have been notified.\n");

    /*
       Wait for both students to finish.
       These are DIFFERENT semaphores from READY.
    */

    sem_wait(done1);
    printf("Student 1 completed.\n");

    sem_wait(done2);
    printf("Student 2 completed.\n");

    printf("\nBoth students completed their calculations.\n");

    /* Cleanup */
    munmap(marks1, SIZE);
    munmap(marks2, SIZE);

    close(fd1);
    close(fd2);

    sem_close(ready1);
    sem_close(ready2);
    sem_close(done1);
    sem_close(done2);

    shm_unlink(SHM1);
    shm_unlink(SHM2);

    sem_unlink(READY1);
    sem_unlink(READY2);
    sem_unlink(DONE1);
    sem_unlink(DONE2);

    printf("Shared memory cleaned up successfully.\n");

    return 0;
}
