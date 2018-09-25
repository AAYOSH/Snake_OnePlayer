# FLAGS=-lncurses -std=c++11 -lpthread 
# 
# all: model
# 
# model: main.cpp modelo.cpp modelo.hpp 
# 	g++ -o model main.cpp modelo.cpp  $(FLAGS)

CC = g++
CFLAGS = -std=c++11 -lpthread 
NCURSES_FLAGS = -lncurses
#PORTAUDIO_FLAGS = -lportaudio -ljack -lrt -lasound -lm
PORTAUDIO_FLAGS = -lportaudio
ASSETS_ZIP = audio/assets/assets.zip
ASSETS_DIR = audio/assets
STAT_DIR = statistics
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

run:all sound
	./$(TARGET)

# sound:
# unzip -n $(ASSETS_ZIP) -d $(ASSETS_DIR)/

clean:
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o
	$(RM) $(ASSETS_DIR)/*.dat
	$(RM) $(STAT_DIR)/*.est