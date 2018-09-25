
CC = g++
CFLAGS = -std=c++11 -lpthread 
NCURSES_FLAGS = -lncurses

PORTAUDIO_FLAGS = -lportaudio
DIR = sources
TARGET = snakegame
SRCS := $(shell find $(DIR) -name '*.cpp')
OBJS = $(SRCS:.c=.o)

.PHONY: depend clean

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(NCURSES_FLAGS) $(PORTAUDIO_FLAGS)

%.o:%.cpp
	$(CC) $(SRCS) $(CFLAGS) -c $< -o $@

run:all 
	./$(TARGET)

clean:
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o