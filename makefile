main: main.cpp
	g++ main.cpp -o main

test:
	./main < example.txt