SRC = src/
DST = bin/


all: $(DST)main.o $(DST)paint.o
	g++ $(DST)main.o $(DST)paint.o -l$(LNAME) -o $(DST)$(NAME)

$(DST)main.o: $(SRC)main.cpp $(SRC)paint.hpp
	mkdir -p $(DST)
	g++ -c $(SRC)main.cpp -o $(DST)main.o

$(DST)paint.o: $(SRC)paint.cpp $(SRC)paint.hpp
	mkdir -p $(DST)
	g++ -c $(SRC)paint.cpp -o $(DST)paint.o

clean:
	rm -rf $(DST)*.*o