macos:
	mkdir -p bin
	g++ -std=c++11 test/test.cpp -D PILATES_DEBUG -Isrc/ -o bin/test -Wno-writable-strings
	./bin/test
.PHONY: macos
