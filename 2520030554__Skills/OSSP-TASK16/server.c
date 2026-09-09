#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CLIENTS 3
#define BUFFER_SIZE 256

const char *client_to_server[MAX_CLIENTS] =
{
    "client1_to_server",
    "client2_to_server",
    "client3_to_server"
};

const char *server_to_client[MAX_CLIENTS] =
{
    "server_to_client1",
    "server_to_client2",
    "server_to_client3"
};

pid_t children[MAX_CLIENTS];

volatile sig_atomic_t server_running = 1;


/* SIGINT handler */
void handle_sigint(int signo)
{
    printf("\nServer: SIGINT received. Shutting down...\n");
    server_running = 0;
}


/* SIGCHLD handler */
void handle_sigchld(int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
        printf("Server: Child process terminated.\n");
    }
}


/* SIGUSR1 handler */
void handle_sigusr1(int signo)
{
    printf("Server: SIGUSR1 received.\n");
}


/* Handler process for each client */
void client_handler(int client_id)
{
    char buffer[BUFFER_SIZE];

    int read_fd;
    int write_fd;

    printf("Handler %d: Started for Client %d\n",
           client_id + 1, client_id + 1);

    /* Client -> Server FIFO */
    read_fd = open(client_to_server[client_id], O_RDONLY);

    if (read_fd == -1)
    {
        perror("open client_to_server");
        exit(EXIT_FAILURE);
    }

    /* Server -> Client FIFO */
    write_fd = open(server_to_client[client_id], O_WRONLY);

    if (write_fd == -1)
    {
        perror("open server_to_client");
        close(read_fd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t bytes = read(read_fd, buffer, BUFFER_SIZE - 1);

        if (bytes <= 0)
        {
            break;
        }

        buffer[bytes] = '\0';

        printf("Server Handler %d received: %s",
               client_id + 1, buffer);

        /* Send response */
        char response[BUFFER_SIZE];

        snprintf(response, BUFFER_SIZE,
                 "Server response to Client %d: %s",
                 client_id + 1, buffer);

        write(write_fd, response, strlen(response));

        printf("Handler %d: Response sent.\n",
               client_id + 1);
    }

    close(read_fd);
    close(write_fd);

    printf("Handler %d: Client disconnected.\n",
           client_id + 1);

    exit(EXIT_SUCCESS);
}


int main()
{
    printf("========== SERVER ==========\n");

    /* Remove old FIFOs if present */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        unlink(client_to_server[i]);
        unlink(server_to_client[i]);
    }

    /* Create FIFOs */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (mkfifo(client_to_server[i], 0666) == -1)
        {
            perror("mkfifo client_to_server");
            exit(EXIT_FAILURE);
        }

        if (mkfifo(server_to_client[i], 0666) == -1)
        {
            perror("mkfifo server_to_client");
            exit(EXIT_FAILURE);
        }
    }

    printf("Server: All FIFOs created.\n");

    /* Save server PID */
    FILE *pid_file = fopen("server.pid", "w");

    if (pid_file == NULL)
    {
        perror("server.pid");
        exit(EXIT_FAILURE);
    }

    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);

    /* Register signal handlers */
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGINT, handle_sigint);
    signal(SIGCHLD, handle_sigchld);

    /* Create three handler processes */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        children[i] = fork();

        if (children[i] == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (children[i] == 0)
        {
            client_handler(i);
        }
    }

    printf("Server: Three handler processes created.\n");
    printf("Server PID: %d\n", getpid());
    printf("Server is running...\n");
    printf("Press Ctrl+C to stop the server.\n");

    /* Wait for signal */
    while (server_running)
    {
        pause();
    }

    /* Terminate handler processes */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (children[i] > 0)
        {
            kill(children[i], SIGTERM);
        }
    }

    /* Wait for children */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        waitpid(children[i], NULL, 0);
    }

    /* Remove FIFOs */
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        unlink(client_to_server[i]);
        unlink(server_to_client[i]);
    }

    unlink("server.pid");

    printf("Server: FIFOs removed.\n");
    printf("Server: Shutdown complete.\n");

    return 0;
}
