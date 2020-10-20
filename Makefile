app:
	rm -rf code && clang++ code.cpp -g -Wall -Wpedantic -fsanitize=memory -std=c++17 -o code && ./code
debug:
	rm -rf code && clang++ code.cpp -g -Wall -Wpedantic -fsanitize=memory -std=c++17 -o code && gdb ./code
clean:
	rm -rf code
