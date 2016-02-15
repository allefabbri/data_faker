all: 
	$(CXX) -std=c++11 -Isrc -o bin/data_faker.exe src/math_lib.c src/data_faker.cpp
	./bin/data_faker.exe
