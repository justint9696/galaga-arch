CC 			= gcc

BIN 		= bin/
OBJ 		= obj/
SRC 		= src/
INC 		= src/

SRCS		= $(shell find $(SRC) -name '*.c') 
OBJS 		= $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

INCLUDES 	= -I$(INC)

CFLAGS 		= -g -O3 -Wall
CFLAGS  	+= $(INCLUDES)
LDFLAGS 	= -lSDL2 -lSDL2_image -lSDL2_ttf -lm

TARGET 		= $(BIN)galaga

all: build

build: $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(LDFLAGS) -o $(TARGET) $^ 

release: build
	strip -s $(TARGET)

run: all
	$(TARGET)

$(OBJ)%.o: $(SRC)%.c $(INC)%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< 

$(OBJ)%.o: $(SRC)%.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
