CC = gcc

BIN = bin/
OBJ = obj/
SRC = src/

SRCS = $(shell find $(SRC) -name '*.c') 
OBJS = $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

INCLUDES = -Iinclude/ -Isrc/

CFLAGS = -g -O3 -Wall -Wextra
CFLAGS += -Wno-unused-parameter
CFLAGS += $(INCLUDES)

LDFLAGS = -lm
LDFLAGS += -lSDL2 -lSDL2_image -lSDL2_ttf

TARGET = $(BIN)galaga

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) -o $@ $^ $(LDFLAGS) 

release: all
	strip -s $(TARGET)

run: all
	@$(TARGET)

$(OBJ)%.o: $(SRC)%.c 
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ)
