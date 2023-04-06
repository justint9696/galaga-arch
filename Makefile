CC=gcc

BIN=bin/
SRC=$(wildcard src/*.c)

CFLAGS=-Wall
LDFLAGS=-L ../ludo -lSDL2 -lSDL2_image -lSDL2_ttf -lm
OBJ=$(SRC:.c=.o)

TARGET=game

all: dir $(TARGET)

dir:
	mkdir -p $(BIN)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN)$(TARGET) $^ $(LDFLAGS)

run: all
	$(BIN)$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -rf $(BIN) $(OBJ)
