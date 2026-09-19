#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#define A2B "fifoA"
#define B2A "fifoB"

int main()
{
    char msg[100];

    mkfifo(A2B, 0666);
    mkfifo(B2A, 0666);

    int pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    // CHILD = RECEIVE FROM A
    if (pid == 0)
    {
        int fd = open(A2B, O_RDONLY);

        while (1)
        {
            int n = read(fd, msg, sizeof(msg) - 1);

            if (n <= 0)
                break;

            msg[n] = '\0';

            printf("\n[User A]: %s\n", msg);
            fflush(stdout);

            if (strcmp(msg, "bye") == 0)
                break;
        }

        close(fd);
        exit(0);
    }

    // PARENT = SEND TO A
    else
    {
        int fd = open(B2A, O_WRONLY);

        while (1)
        {
            printf("User B: ");
            fflush(stdout);

            if (fgets(msg, sizeof(msg), stdin) == NULL)
                break;

            msg[strcspn(msg, "\n")] = '\0';

            write(fd, msg, strlen(msg) + 1);

            if (strcmp(msg, "bye") == 0)
                break;
        }

        close(fd);
    }

    return 0;
}
