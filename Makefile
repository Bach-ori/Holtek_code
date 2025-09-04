CC = gcc
CFLAGS = -Wall -I./My_lib
LDFLAGS = -lgpiod    # <- linker flag

SRC = main.c $(wildcard My_lib/*.c)
OBJ = $(SRC:.c=.o)
TARGET = main

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)   

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
