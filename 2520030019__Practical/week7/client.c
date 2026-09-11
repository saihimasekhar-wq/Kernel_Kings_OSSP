#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

int client_no;

char c2s[100];
char s2c[100];

int read_fd;
int write_fd;

void sigusr1_handler(int sig)
{
    printf("\nClient %d: SIGUSR1 received\n", client_no);
}

void sigint_handler(int sig)
{
    printf("\nClient %d shutting down...\n", client_no);

    close(write_fd);
    close(read_fd);

    exit(0);
}

int main(int argc, char *argv[])
{
    char message[256];
    char response[256];

    if (argc != 2) {
        printf("Usage: %s <client_number>\n", argv[0]);
        printf("Example: %s 1\n", argv[0]);
        return 1;
    }

    client_no = atoi(argv[1]);

    if (client_no < 1 || client_no > 3) {
        printf("Client number must be 1, 2 or 3\n");
        return 1;
    }

    sprintf(c2s, "client%d_to_server", client_no);
    sprintf(s2c, "server_to_client%d", client_no);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGINT, sigint_handler);

    /*
     * O_RDWR avoids FIFO open deadlock.
     */

    write_fd = open(c2s, O_RDWR);
    read_fd = open(s2c, O_RDWR);

    if (write_fd < 0 || read_fd < 0) {
        perror("open");
        return 1;
    }

    printf("\nCLIENT %d STARTED\n", client_no);
    printf("Client PID = %d\n", getpid());

    while (1) {

        printf("Client %d > ", client_no);

        fgets(message, sizeof(message), stdin);

        /* Send message to server */

        write(write_fd, message, strlen(message));

        /* Read response */

        memset(response, 0, sizeof(response));

        int n = read(read_fd, response, sizeof(response) - 1);

        if (n > 0) {

            response[n] = '\0';

            printf("Response: %s", response);
        }

        if (strncmp(message, "exit", 4) == 0)
            break;
    }

    close(write_fd);
    close(read_fd);

    printf("Client %d terminated.\n", client_no);

    return 0;
}
