SRC = src/
LIB_SRC = ../src/
DST = out/

all: $(DST)key.o $(DST)terminal.o
	g++ $(SRC)main.cpp $(DST)key.o $(DST)terminal.o -o $(DST)libtest

$(DST)key.o: $(LIB_SRC)key.cpp $(LIB_SRC)key.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)key.cpp -o $(DST)key.o

$(DST)terminal.o: $(LIB_SRC)terminal.cpp $(LIB_SRC)terminal.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)terminal.cpp -o $(DST)terminal.o

clean:
	rm -rf $(DST)*.o