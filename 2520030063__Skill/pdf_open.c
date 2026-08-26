#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("Web Browser process started.\n");
    printf("Browser PID: %d\n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        printf("Child process created.\n");
        printf("Child PID: %d\n", getpid());

        printf("Opening PDF using external viewer...\n");

        execlp("xdg-open", "xdg-open", "notes.pdf", (char *)NULL);

        perror("execlp failed");
        exit(1);
    }
    else
    {
        printf("Browser is still running.\n");
        printf("Parent PID: %d\n", getpid());

        wait(NULL);

        printf("PDF viewer process finished.\n");
    }

    return 0;
}
