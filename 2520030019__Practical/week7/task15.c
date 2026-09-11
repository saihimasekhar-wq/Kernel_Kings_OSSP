#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t producer, consumer;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    // Create Producer Process
    producer = fork();

    if (producer < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (producer == 0) {
        // Producer Child Process

        // Redirect stdout to write end of pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            exit(EXIT_FAILURE);
        }

        // Close unused pipe ends
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute: ls -l
        execlp("ls", "ls", "-l", NULL);

        // Executes only if execlp fails
        perror("execlp ls failed");
        exit(EXIT_FAILURE);
    }

    // Create Consumer Process
    consumer = fork();

    if (consumer < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (consumer == 0) {
        // Consumer Child Process

        // Redirect stdin to read end of pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2 failed");
            exit(EXIT_FAILURE);
        }

        // Close unused pipe ends
        close(pipefd[1]);
        close(pipefd[0]);

        // Execute: grep ".c"
        execlp("grep", "grep", ".c", NULL);

        // Executes only if execlp fails
        perror("execlp grep failed");
        exit(EXIT_FAILURE);
    }

    // Parent Process

    // Close both pipe ends
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both child processes
    waitpid(producer, NULL, 0);
    waitpid(consumer, NULL, 0);

    return 0;
}
