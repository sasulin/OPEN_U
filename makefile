TARGET = main
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -ansi -pedantic

.PHONY: default all clean

all: default
default: $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -ansi -pedantic $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
