CC			:= gcc

BIN			:= bin
SRC			:= src

SRCS		:= $(wildcard $(SRC)/*.c)
OBJS 		:= $(SRCS:$(SRC)/%.c=$(BIN)/%.o)

CFLAGS 		:= -Wall
LDFLAGS 	:= -L ../ludo -lSDL2 -lSDL2_image -lSDL2_ttf -lm

TARGET		:= galaga

all: dir $(TARGET)

dir:
	mkdir -p $(BIN)

$(TARGET): $(OBJS)
	$(CC) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)

run: all
	$(BIN)/$(TARGET)

$(BIN)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	rm -rf $(BIN) $(OBJ)
