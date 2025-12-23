TARGET = main.out


SRCS = $(wildcard *.c)

CC = gcc
CFLAGS = -Wall -Wextra -g

all: $(TARGET)


$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
