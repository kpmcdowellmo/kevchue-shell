CC = gcc
CFLAGS = -Wall -g -I ./lib
SRC = main.c ./lib/commands.c
OBJ = $(SRC:.c=.o)
TARGET = kevchue-shell

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJCS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
