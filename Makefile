CXX := g++
CC := gcc
CXXFLAGS := -std=c++14 -O2 -Wall -Wextra -Wpedantic
CFLAGS := -O2 -Wall -Wextra
INC := -Iinclude -Ilib/fips202

CORE := src/uint512.cpp src/modular.cpp
PRNG := lib/fips202/prng.cpp
FIPS := lib/fips202/fips202.c

.PHONY: all test run clean

all: bin/dh

bin/fips202.o: $(FIPS)
	mkdir -p bin
	$(CC) $(CFLAGS) -Ilib/fips202 -c $(FIPS) -o bin/fips202.o

bin/test_modular: $(CORE) tests/test_modular.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INC) $(CORE) tests/test_modular.cpp -o bin/test_modular

bin/dh: $(CORE) src/prime.cpp src/main.cpp $(PRNG) bin/fips202.o
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INC) $(CORE) src/prime.cpp src/main.cpp $(PRNG) bin/fips202.o -o bin/dh

test: bin/test_modular
	./bin/test_modular

run: bin/dh
	./bin/dh

clean:
	rm -rf bin
