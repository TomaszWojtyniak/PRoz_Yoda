
SOURCES=$(wildcard *.cpp)
HEADERS=$(SOURCES:.c=.h)
FLAGS=-DDEBUG -g

all: main

main: $(SOURCES) $(HEADERS)
	mpic++ $(SOURCES) $(FLAGS) -o main -lpthread

clean:
	rm main

run: main
	mpirun -np 8 ./main
