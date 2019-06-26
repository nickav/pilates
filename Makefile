macos:
	mkdir -p bin
	g++ -std=c++11 src/main.cpp -o bin/main -Wno-writable-strings
	./bin/main
