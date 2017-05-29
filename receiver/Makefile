CC = gcc
DEBUG = -g3
CFLAGS += -Wall
CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += $(shell pkg-config --cflags libmongoc-1.0)
LDFLAGS += $(shell pkg-config --libs json-c)
TARGET = receiver
LIBS =  -lpthread -lmongoc-1.0 -lbson-1.0 -ljson-c
all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(DEBUG) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
