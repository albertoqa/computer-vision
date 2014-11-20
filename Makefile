CC=g++
CXXFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: vc clean

vc: main.o hybrid.o panorama.o
	$(CC) -o executable $(LIBS) main.o hybrid.o panorama.o

main.o:
	$(CC) $(CXXFLAGS) -c ./src/main.cpp

hybrid.o:
	$(CC) $(CXXFLAGS) -c ./src/hybrid.cpp

panorama.o:
	$(CC) $(CXXFLAGS) -c ./src/panorama.cpp

clean:
	rm -rf *.o
