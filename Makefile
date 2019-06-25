macos:
	mkdir -p bin
	clang++ src/main.cpp -o bin/main -Wno-writable-strings
	./bin/main
