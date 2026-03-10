
COMPILER_FLAGS = -g -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -std=c++23 -Iinclude

MAKEFLAGS += -j4 # WARNING this flag enables multi thread compile. This only works if dependencies are declared correctly in this file

build: bin/final # produces executable file for the whole application, if not already produced

bin/Tests.o: include/ScanValue.hpp include/TimeValue.hpp include/ITimer.hpp
	@echo "-> compiling Tests"
	g++ -c tests/Tests.cpp -o bin/Tests.o $(COMPILER_FLAGS)

bin/final: bin/Tests.o # links
	@echo "-> linking"
	g++ bin/Tests.o -o bin/final $(COMPILER_FLAGS)

clean:
	@echo "-> cleaning bin folder"
	rm -f  bin/*
