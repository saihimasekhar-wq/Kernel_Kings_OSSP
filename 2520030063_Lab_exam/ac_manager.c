
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/ac_fifo"

int main() {
    char buffer[100];

    // Create FIFO if it does not exist
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        if (access(FIFO_PATH, F_OK) == -1) {
            perror("mkfifo");
            return 1;
        }
    }

    printf("Air Conditioner Manager Started\n");
    printf("Waiting for setpoint updates...\n");

    // Open FIFO for reading
    int fd = open(FIFO_PATH, O_RDONLY);
    
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ssize_t bytes;

    while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("Received setpoint: %s", buffer);
    }

    close(fd);
    return 0;
}

