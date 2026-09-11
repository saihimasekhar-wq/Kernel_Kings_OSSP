#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    printf("Start\n");

    pid = fork();   // Create a child process

    if (pid == 0) {   // Child process
        printf("Start of child\n");

        sleep(15);    // Child sleeps for 15 seconds

        printf("End of child\n");

        exit(0);      // Child process terminates
    }
    else if (pid > 0) {   // Parent process
        sleep(10);        // Parent sleeps for 10 seconds

        wait(NULL);       // Wait for child process to finish

        printf("End\n");
    }
    else {
        printf("Fork failed.\n");
    }

    return 0;
}
/*
OUTPUT:
Start
Start of child
End of child
End
*/
