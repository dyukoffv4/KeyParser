SRC = src/

all: $(DST)lib$(NAME).so

$(DST)lib$(NAME).so: $(DST)key.o $(DST)parser.o
	g++ -shared $(DST)key.o $(DST)parser.o -o $(DST)lib$(NAME).so

$(DST)key.o: $(SRC)key.cpp $(SRC)key.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)key.cpp -o $(DST)key.o

$(DST)parser.o: $(SRC)parser.cpp $(SRC)parser.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)parser.cpp -o $(DST)parser.o

clean:
	rm -rf $(DST)*.*o