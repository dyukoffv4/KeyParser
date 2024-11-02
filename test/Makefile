SRC = src/
LIB_SRC = ../src/
DST = out/

# Add test goal there
all: $(DST)test_task_1

$(DST)test_task_1: $(SRC)test_task_1.cpp $(DST)key.o $(DST)tasks.o $(DST)parser.o
	g++ $(SRC)test_task_1.cpp $(DST)key.o $(DST)tasks.o $(DST)parser.o -o $(DST)test_task_1

# Don't change code below
$(DST)key.o: $(LIB_SRC)key.cpp $(LIB_SRC)key.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)key.cpp -o $(DST)key.o

$(DST)tasks.o: $(LIB_SRC)tasks.cpp $(LIB_SRC)tasks.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)tasks.cpp -o $(DST)tasks.o

$(DST)parser.o: $(LIB_SRC)parser.cpp $(LIB_SRC)parser.hpp
	mkdir -p $(DST)
	g++ -c $(LIB_SRC)parser.cpp -o $(DST)parser.o

clean:
	rm -rf $(DST)*.o