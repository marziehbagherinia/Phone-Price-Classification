#include "LoadBalancer.h"

int main(int argc, char const *argv[])
{
	string datasets_directory = argv[1];

    //auto start_0 = high_resolution_clock::now();
	
	load_balancer(datasets_directory);
	set_extermum();
	normalize();
	classification();
	compute_accuracy();

	//auto stop_0 = high_resolution_clock::now();
    //auto duration_0 = duration_cast<microseconds>(stop_0 - start_0);
    //cout << "Time 1: "<< duration_0.count() << " ms" << endl;

    pthread_exit(NULL);
	return 0;
}
