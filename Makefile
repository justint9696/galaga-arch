CC 			= gcc

BIN 		= bin/
OBJ 		= obj/
SRC 		= src/

SRCS		= $(shell find $(SRC) -name '*.c') 
OBJS 		= $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

INCLUDES 	= -I$(SRC)

CFLAGS 		= -g -O3 -Wall
CFLAGS  	+= $(INCLUDES)
LDFLAGS 	= -L ../ludo -lSDL2 -lSDL2_image -lSDL2_ttf -lm

TARGET 		= $(BIN)galaga

all: build

build: $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(LDFLAGS) -o $(TARGET) $^ 

run: all
	$(TARGET)

$(OBJ)%.o: $(SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -rf $(BIN) $(OBJ)
