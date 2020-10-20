app:
	rm -rf code && clang++ code.cpp -g -Wall -Wpedantic -fsanitize=memory -std=c++20 -o code && ./code
debug:
	rm -rf code && clang++ code.cpp -g -Wall -Wpedantic -fsanitize=memory -std=c++20 -o code && gdb ./code
clean:
	rm -rf code
