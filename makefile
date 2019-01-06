CC = g++

CFLAGS = -w -lSDL2 -lSDL2_image

TARGET = sprite_test

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(TARGET).cpp $(CFLAGS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
