CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

TARGET = forgeos_monitor

SRC = \
	src/main.c \
	src/utils.c \
	src/system_info.c \
	src/cpu_monitor.c \
	src/memory_monitor.c \
	src/disk_monitor.c \
	src/process_monitor.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
