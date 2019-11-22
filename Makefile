all: main

main: main.cpp
	g++ main.cpp scanner.cpp parser.cpp testTree.cpp statSem.cpp -o compile
	
clean:
	rm compile kb.fs19