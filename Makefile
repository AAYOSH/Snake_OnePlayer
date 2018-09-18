FLAGS=-lncurses -std=c++11 -lpthread 

all: model

model: main.cpp modelo.cpp modelo.hpp 
	g++ -o model main.cpp modelo.cpp  $(FLAGS)
