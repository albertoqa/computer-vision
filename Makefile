CC=g++
CXXFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: vc clean

vc: main.o blend.o aux.o hybrid.o
	$(CC) -o executable $(LIBS) main.o blend.o aux.o hybrid.o

main.o:
	$(CC) $(CXXFLAGS) -c ./src/main.cpp

blend.o:
	$(CC) $(CXXFLAGS) -c ./src/blend.cpp

aux.o:
	$(CC) $(CXXFLAGS) -c ./src/aux.cpp

hybrid.o:
	$(CC) $(CXXFLAGS) -c ./src/hybrid.cpp

clean:
	rm -rf *.o
