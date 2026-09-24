#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_CLIENTS 3

char *c2s[MAX_CLIENTS] = {
    "client1_to_server",
    "client2_to_server",
    "client3_to_server"
};

char *s2c[MAX_CLIENTS] = {
    "server_to_client1",
    "server_to_client2",
    "server_to_client3"
};

void sigusr1_handler(int sig)
{
    printf("Server: SIGUSR1 received\n");
}

void sigint_handler(int sig)
{
    printf("\nServer: SIGINT received. Shutting down...\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        unlink(c2s[i]);
        unlink(s2c[i]);
    }

    exit(0);
}

void sigchld_handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    printf("Server: Child process terminated\n");
}

void client_handler(int client_no)
{
    char buffer[256];

    printf("Handler %d started. PID = %d\n",
           client_no + 1, getpid());

    int read_fd = open(c2s[client_no], O_RDWR);
    int write_fd = open(s2c[client_no], O_RDWR);

    if (read_fd < 0 || write_fd < 0) {
        perror("open");
        exit(1);
    }

    while (1) {

        memset(buffer, 0, sizeof(buffer));

        int n = read(read_fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {

            buffer[n] = '\0';

            printf("Handler %d received: %s",
                   client_no + 1, buffer);

            if (strncmp(buffer, "exit", 4) == 0) {
                char response[] = "Server: Goodbye!\n";
                write(write_fd, response, strlen(response));
                break;
            }

            char response[300];

            snprintf(response, sizeof(response),
                     "Server Handler %d: Message received -> %s",
                     client_no + 1, buffer);

            write(write_fd, response, strlen(response));
        }
    }

    close(read_fd);
    close(write_fd);

    printf("Handler %d exiting\n", client_no + 1);

    exit(0);
}

int main()
{
    printf("SERVER STARTED\n");
    printf("Server PID = %d\n", getpid());

    /* Install signal handlers */

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchld_handler);

    /* Create FIFOs */

    for (int i = 0; i < MAX_CLIENTS; i++) {

        mkfifo(c2s[i], 0666);
        mkfifo(s2c[i], 0666);
    }

    printf("All FIFOs created.\n");

    /* Create one handler for each client */

    for (int i = 0; i < MAX_CLIENTS; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            client_handler(i);
        }

        printf("Created Handler %d with PID %d\n",
               i + 1, pid);
    }

    /* Server main process */

    while (1) {
        pause();
    }

    return 0;
}
