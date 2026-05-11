all: parser

parser: main.cpp
	g++ -std=c++11 -Wall main.cpp -o parser
			

clean:
	rm -f parser
