CC=g++
CF=-std=c++11
all : fileDir PhonePricePrediction.out 

fileDir:
	@mkdir -p build
	@mkdir -p bin

	
# LoadBalancer
PhonePricePrediction.out :  build/Main.o build/LoadBalancer.o
	$(CC) $(CF) build/Main.o build/LoadBalancer.o -lpthread -lrt -o  PhonePricePrediction.out

build/Main.o : src/Main.cpp src/LoadBalancer.h src/LoadBalancer.cpp 
	$(CC) $(CF) -c src/Main.cpp -o build/Main.o
	
build/LoadBalancer.o : src/LoadBalancer.cpp src/LoadBalancer.h
	$(CC) $(CF) -c src/LoadBalancer.cpp -o build/LoadBalancer.o

clean :
	rm -f -r build
	rm -f -r bin
	rm -f PhonePricePrediction.out
