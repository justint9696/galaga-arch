CC = gcc

BIN = bin/
OBJ = obj/
SRC = src/
LIB = lib/

SRCS = $(shell find $(SRC) -name '*.c') 
OBJS = $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

INCLUDES = -Iinclude/ -Isrc/

CFLAGS = -g -O3 -Wall $(INCLUDES)

LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf

TARGET = $(BIN)galaga

.PHONY: all clean

all: build

win32: 
	@mkdir -p $(BIN)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) -L$(LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

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

clean:
	rm -rf $(TARGET) $(OBJ)
