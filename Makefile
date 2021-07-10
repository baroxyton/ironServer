./build/ironServer: ./src/server/server.cpp
	g++ -pthread ./src/server/server.cpp -O3 -o ./build/ironServer
run:
	./build/ironServer
clean:
	rm -f ./build/ironServer
