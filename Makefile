app:
	rm -rf code && clang++-10 code.cpp -g -Wall -Wpedantic -std=c++20 -o code && ./code
debug:
	rm -rf code && clang++-10 code.cpp -g -Wall -Wpedantic -std=c++20 -o code && gdb ./code
clean:
	rm -rf code
