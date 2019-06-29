macos:
	mkdir -p bin
	g++ -std=c++11 src/main.cpp -o bin/main -Wno-writable-strings
	./bin/main

test:
	mkdir -p bin
	g++ -std=c++11 test/test.cpp -Isrc/ -o bin/test -Wno-writable-strings
	./bin/test

.PHONY: test
.PHONY: macos
