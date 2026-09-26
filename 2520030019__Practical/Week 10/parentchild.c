#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main() {

    int *count;

    // Create shared memory
    count = mmap(NULL, sizeof(int),
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS,
                 -1, 0);

    if (count == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    *count = 0;

    int pid = fork();

    if (pid < 0) {
        perror("Problem in creating child");
        exit(1);
    }

    else if (pid == 0) {

        printf("Child process is executing\n");

        for (int i = 0; i < 5; i++) {
            int temp = *count;
            temp--;
            *count = temp;

            printf("child count: %d\n", *count);
        }
    }

    else {

        printf("Parent process is executing\n");

        for (int i = 0; i < 5; i++) {
            int temp = *count;
            temp--;
            *count = temp;

            printf("parent count: %d\n", *count);
        }

        wait(NULL);

        printf("\nFinal count: %d\n", *count);

        // Remove shared memory
        munmap(count, sizeof(int));
    }

    return 0;
}
