all: 
	$(CXX) -std=c++11 -o data_faker.exe math_lib.c data_faker.cpp
	./data_faker.exe
