
COMPILER_FLAGS = -g -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -std=c++23

MAKEFLAGS += -j4 # WARNING this flag enables multi thread compile. This only works if dependencies are declared correctly in this file

build: bin/final # produces executable file for the whole application, if not already produced

bin/ScanValue.o: src/ScanValue.cpp include/ScanValue.hpp
	@echo "-> compiling ScanValue.cpp"
	g++ -c src/ScanValue.cpp -o bin/ScanValue.o $(COMPILER_FLAGS)

bin/final: bin/ScanValue.o # links
	@echo "-> linking"
	g++ bin/ScanValue.o -o bin/final $(COMPILER_FLAGS)

clean:
	@echo "-> cleaning bin folder"
	rm -f  bin/*
