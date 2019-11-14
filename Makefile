all: main

main: main.cpp
	g++ main.cpp scanner.cpp parser.cpp testTree.cpp statSem.cpp -o statSem
	
clean:
	rm statSem input.fs19