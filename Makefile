build:
	@g++ main.cpp container.cpp node.cpp xml.cpp -o ./bin/main -std=c++20
run: build
	@./bin/main
