CC 			= gcc

BIN			= bin/
OBJ			= obj/

SRCS		= $(wildcard src/*.c src/*/*.c)
OBJS 		= $(patsubst src/%, $(OBJ)%, $(SRCS:.c=.o))

CFLAGS 		= -Wall
LDFLAGS 	= -L ../ludo -lSDL2 -lSDL2_image -lSDL2_ttf -lm

TARGET 		= $(BIN)galaga

all: build

build: $(OBJS)
	mkdir -p $(BIN)
	$(CC) -o $(TARGET) $^ $(LDFLAGS)

run: all
	$(TARGET)

$(OBJ)%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) 

clean:
	rm -rf $(BIN) $(OBJS)
