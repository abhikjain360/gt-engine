app:
	rm -rf code && clang++ code.cpp -g -Wall -fsanitize=address -Wpedantic -std=c++20 -o code && ./code
debug:
	rm -rf code && clang++ code.cpp -g -Wall -fsanitize=address -Wpedantic -std=c++20 -o code && gdb ./code
clean:
	rm -rf code
