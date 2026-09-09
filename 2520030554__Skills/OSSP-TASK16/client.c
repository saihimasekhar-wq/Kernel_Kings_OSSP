#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define BUFFER_SIZE 256

pid_t receiver_pid;

int write_fd;
int read_fd;
pid_t server_pid;


/* SIGINT handler */
void handle_sigint(int signo)
{
    printf("\nClient: SIGINT received. Exiting...\n");

    if (receiver_pid > 0)
    {
        kill(receiver_pid, SIGTERM);
    }

    close(write_fd);
    close(read_fd);

    exit(0);
}


/* Receive messages from server */
void receiver(int fd)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t bytes = read(fd, buffer, BUFFER_SIZE - 1);

        if (bytes <= 0)
        {
            break;
        }

        buffer[bytes] = '\0';

        printf("\n%s\n", buffer);
        printf("Enter message: ");
        fflush(stdout);
    }

    exit(0);
}


int main(int argc, char *argv[])
{
    int client_id;

    char client_to_server[50];
    char server_to_client[50];

    char buffer[BUFFER_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <client_number>\n", argv[0]);
        printf("Example: %s 1\n", argv[0]);
        return 1;
    }

    client_id = atoi(argv[1]);

    if (client_id < 1 || client_id > 3)
    {
        printf("Client number must be 1, 2, or 3.\n");
        return 1;
    }

    sprintf(client_to_server,
            "client%d_to_server", client_id);

    sprintf(server_to_client,
            "server_to_client%d", client_id);

    printf("========== CLIENT %d ==========\n",
           client_id);

    /* Read server PID */
    FILE *pid_file = fopen("server.pid", "r");

    if (pid_file == NULL)
    {
        perror("server.pid");
        return 1;
    }

    fscanf(pid_file, "%d", &server_pid);
    fclose(pid_file);

    /* Client -> Server */
    write_fd = open(client_to_server, O_WRONLY);

    if (write_fd == -1)
    {
        perror("open client_to_server");
        return 1;
    }

    /* Server -> Client */
    read_fd = open(server_to_client, O_RDONLY);

    if (read_fd == -1)
    {
        perror("open server_to_client");
        close(write_fd);
        return 1;
    }

    printf("Connected to server.\n");

    /* Handle Ctrl+C */
    signal(SIGINT, handle_sigint);

    /* Create receiver process */
    receiver_pid = fork();

    if (receiver_pid == -1)
    {
        perror("fork");
        return 1;
    }

    /* Child receives server responses */
    if (receiver_pid == 0)
    {
        close(write_fd);

        receiver(read_fd);
    }

    /* Parent sends messages */
    close(read_fd);

    while (1)
    {
        printf("Enter message: ");
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }

        /* Send message to server */
        write(write_fd, buffer, strlen(buffer));

        /* Notify server */
        kill(server_pid, SIGUSR1);
    }

    close(write_fd);

    kill(receiver_pid, SIGTERM);

    return 0;
}
