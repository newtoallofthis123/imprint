build:
	@g++ main.cpp imprint.cpp -o ./bin/main -std=c++20
run: build
	@./bin/main
