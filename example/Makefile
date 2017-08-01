CC=g++
SRC=$(wildcard *.cpp) $(wildcard ../scr/*.cpp) $(wildcard ../scr/soil/*.c) 
EXEC=../bin/OGSBF
CFLAGS=-std=c++11
LDFLAGS=-lglut -lGLU -lGL

all: make run

make:
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC) $(LDFLAGS)

run:
	$(EXEC)