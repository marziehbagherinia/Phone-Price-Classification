all : fileDir PhonePricePrediction.out 

fileDir:
	@mkdir -p build
	@mkdir -p bin

	
# LoadBalancer
PhonePricePrediction.out :  build/Main.o build/LoadBalancer.o

	g++ build/Main.o build/LoadBalancer.o -o  PhonePricePrediction.out

build/Main.o : src/Main.cpp src/LoadBalancer.h src/LoadBalancer.cpp 
	g++ -c src/Main.cpp -o build/Main.o
	
build/LoadBalancer.o : src/LoadBalancer.cpp src/LoadBalancer.h
	g++ -c src/LoadBalancer.cpp -o build/LoadBalancer.o

clean :
	rm -f -r build
	rm -f -r bin
	rm -f PhonePricePrediction.out
