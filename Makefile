TARGET = bin/main.out


SRCS = $(wildcard src/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address

all: $(TARGET)


$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
