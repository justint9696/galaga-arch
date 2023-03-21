CC=gcc
CFLAGS=-Wall
LDFLAGS=-L ../ludo -lSDL2
OUTPUT_FILES=app.o entity.o main.o
TARGET=run

all: $(TARGET)

$(TARGET): $(OUTPUT_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OUTPUT_FILES) $(LDFLAGS)

clean:
	rm -f $(OUTPUT_FILES) $(TARGET)
