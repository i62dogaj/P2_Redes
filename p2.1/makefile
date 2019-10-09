CPPFLAGS = -g -O3 -std=c++11 -lsqlite3 -DNDEBUG
BIN = main

#main:
#	g++ main.cpp -o main -lsqlite3

all:
	g++ cliente.cpp -o cliente $(CPPFLAGS)
	g++ servidor.cpp -o servidor $(CPPFLAGS)

#$(BIN): *.cpp *.hpp
#	g++ $(BIN).cpp -o $(BIN) $(CPPFLAGS)

clean:
	@rm -rf servidor cliente