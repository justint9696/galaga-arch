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

all: $(TARGET)

win32: LDFLAGS = -L$(LIB)x86 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
win32: $(OBJS)
	@cp -rf $(LIB)x86/bin .
	$(CC) -o $@ $^ $(LDFLAGS) 

win64: LDFLAGS = -L$(LIB)x64 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
win64: $(OBJS)
	@cp -rf $(LIB)x64/bin .
	$(CC) -o $(TARGET) $^ $(LDFLAGS) 

$(TARGET): $(OBJS)
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
	rm -rf $(BIN) $(OBJ)
