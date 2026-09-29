CC = gcc
CFLAGS = -Wall -O2 -g
TARGET = v4l2_capture
SRC = v4l2_capture.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
