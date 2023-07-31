CC 			= gcc

BIN			= bin/

SRCS		= $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJS 		= $(SRCS:%.c=%.o)

CFLAGS 		= -Wall
LDFLAGS 	= -L ../ludo -lSDL2 -lSDL2_image -lSDL2_ttf -lm

TARGET 		= $(BIN)galaga

all: dir build

dir:
	mkdir -p $(BIN)

build: $(OBJS)
	$(CC) -o $(TARGET) $^ $(LDFLAGS)

run: all
	$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -rf $(BIN) $(OBJS)
