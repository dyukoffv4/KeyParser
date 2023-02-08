SRC = src/
DST = bin/


all: $(DST)lib$(NAME).so
	return 0

$(DST)lib$(NAME).so: $(DST)key.o $(DST)terminal.o
	g++ -shared $(DST)key.o $(DST)terminal.o -o $(DST)lib$(NAME).so

$(DST)key.o: $(SRC)key.cpp $(SRC)key.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)key.cpp -o $(DST)key.o

$(DST)terminal.o: $(SRC)terminal.cpp $(SRC)terminal.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)terminal.cpp -o $(DST)terminal.o

clean:
	rm -rf $(DST)*.*o