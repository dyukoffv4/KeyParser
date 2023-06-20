SRC = src/
LIB_SRC = ../src/
DST = out/

all: $(DST)key.o $(DST)parser.o
	g++ $(SRC)main.cpp $(DST)key.o $(DST)parser.o -o $(DST)libtest

$(DST)key.o: $(LIB_SRC)key.cpp $(LIB_SRC)key.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)key.cpp -o $(DST)key.o

$(DST)parser.o: $(LIB_SRC)parser.cpp $(LIB_SRC)parser.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)parser.cpp -o $(DST)parser.o

clean:
	rm -rf $(DST)*.o