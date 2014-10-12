CC=g++
CXXFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: vc clean

vc: main.o blend.o aux.o
	$(CC) -o executable $(LIBS) main.o blend.o aux.o

main.o:
	$(CC) $(CXXFLAGS) -c ./src/main.cpp

blend.o:
	$(CC) $(CXXFLAGS) -c ./src/blend.cpp

aux.o:
	$(CC) $(CXXFLAGS) -c ./src/aux.cpp

clean:
	rm -rf *.o
