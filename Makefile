app:
	rm -rf code && clang++ code.cpp -g -Wall -Wextra -Wpedantic -fsanitizer=address -std=c++20 -o code && ./code
debug:
	rm -rf code && clang++ code.cpp -g -Wall -Wextra -Wpedantic -fsanitizer=address -std=c++20 -o code && gdb ./code
clean:
	rm -rf code
