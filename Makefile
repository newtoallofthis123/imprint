build:
	@g++ main.cpp imprint.cpp xml.cpp -o ./bin/main -std=c++20
run: build
	@./bin/main
