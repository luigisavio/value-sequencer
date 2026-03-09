
COMPILER_FLAGS = -g -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -std=c++23 -Iinclude

MAKEFLAGS += -j4 # WARNING this flag enables multi thread compile. This only works if dependencies are declared correctly in this file

build: bin/final # produces executable file for the whole application, if not already produced

bin/ScanValueTests.o: tests/ScanValueTests.cpp include/ScanValue.hpp
	@echo "-> compiling ScanValueTests.cpp"
	g++ -c tests/ScanValueTests.cpp -o bin/ScanValue.o $(COMPILER_FLAGS)

bin/final: bin/ScanValueTests.o # links
	@echo "-> linking"
	g++ bin/ScanValue.o -o bin/final $(COMPILER_FLAGS)

clean:
	@echo "-> cleaning bin folder"
	rm -f  bin/*
