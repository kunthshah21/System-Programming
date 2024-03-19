CC = gcc
CFLAGS = -Wall -Wextra

TARGET = my_cmd

.PHONY: all clean

all: $(TARGET)

$(TARGET): my_cmd.c
    $(CC) $(CFLAGS) -o $(TARGET) my_cmd.c

clean:
    rm -f $(TARGET)
