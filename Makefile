SRC = src/

all: $(DST)lib$(NAME).so

$(DST)lib$(NAME).so: $(DST)key.o $(DST)parser.o $(DST)tasks.o $(DST)binds.o
	g++ -shared $(DST)key.o $(DST)parser.o $(DST)tasks.o $(DST)binds.o -o $(DST)lib$(NAME).so

$(DST)key.o: $(SRC)key.cpp $(SRC)key.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)key.cpp -o $(DST)key.o

$(DST)tasks.o: $(SRC)tasks.cpp $(SRC)tasks.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)tasks.cpp -o $(DST)tasks.o

$(DST)parser.o: $(SRC)parser.cpp $(SRC)parser.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)parser.cpp -o $(DST)parser.o

$(DST)binds.o: $(SRC)binds.cpp $(SRC)binds.hpp $(SRC)defines.hpp
	mkdir -p $(DST)
	g++ -c -Wall -fPIC $(SRC)binds.cpp -o $(DST)binds.o

clean:
	rm -rf $(DST)*.*o