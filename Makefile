build:
	@g++ main.cpp -o ./bin/main -std=c++20
run: build
	@./bin/main
