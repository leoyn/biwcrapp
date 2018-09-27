all: tools/main.cpp
	g++ tools/main.cpp -ljsoncpp -ltinyxml2 -lcurl -pthread -std=c++11 -o tools/main

clean:
	rm -rf dependencies
