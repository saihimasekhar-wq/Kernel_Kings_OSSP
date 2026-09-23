#include "utils.h"

#include <fcntl.h>
#include <unistd.h>

int read_file(const char *path, char *buffer, size_t size)
{
    if (path == NULL || buffer == NULL || size == 0)
    {
        return -1;
    }

    int fd = open(path, O_RDONLY);

    if (fd == -1)
    {
        return -1;
    }

    ssize_t total_read = 0;

    while (total_read < (ssize_t)(size - 1))
    {
        ssize_t bytes_read = read(
            fd,
            buffer + total_read,
            size - 1 - total_read
        );

        if (bytes_read == -1)
        {
            close(fd);
            return -1;
        }

        if (bytes_read == 0)
        {
            break;
        }

        total_read += bytes_read;
    }

    buffer[total_read] = '\0';

    close(fd);

    return (int)total_read;
}
