CC 			= gcc

BIN 		= bin/
OBJ 		= obj/
SRC 		= src/
INC 		= src/
LIB 		= lib/

SDL 		= $(LIB)SDL/

SRCS		= $(shell find $(SRC) -name '*.c') 
OBJS 		= $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

INCLUDES 	= -I$(INC) \
			  -I$(SDL)include

CFLAGS 		= -g -O3 -Wall
CFLAGS  	+= $(INCLUDES)

LDFLAGS 	= -lm \
			  -L$(BIN)SDL -lSDL2 \
			  -L$(BIN)SDL_image -lSDL2_image \
			  -L$(BIN)SDL_ttf -lSDL2_ttf 

TARGET 		= $(BIN)galaga

.PHONY: all clean
all: build

libs: sdl sdl_image sdl_ttf

sdl:
	mkdir -p $(BIN)SDL
	cmake -B $(BIN)SDL -S $(SDL) 
	cd $(BIN)SDL && make -j 10

sdl_image:
	mkdir -p $(BIN)SDL_image
	cmake -B $(BIN)SDL_image -S $(SDL) 
	cd $(BIN)SDL_image && make -j 10

sdl_ttf:
	mkdir -p $(BIN)SDL_ttf
	cmake -B $(BIN)SDL_ttf -S $(SDL) 
	cd $(BIN)SDL_ttf && make -j 10

build: $(OBJS)
	@mkdir -p $(BIN)
	$(CC) -o $(TARGET) $^ $(LDFLAGS) 

release: all
	strip -s $(TARGET)

run: all
	$(TARGET)

$(OBJ)%.o: $(SRC)%.c 
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJ)
