TARGET = bin/main.out

SRCS = $(wildcard src/*.c)

CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address

all: $(TARGET)

$(TARGET): | bin
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

bin:
	mkdir -p bin

clean:
	rm -f $(TARGET)

.PHONY: all clean
