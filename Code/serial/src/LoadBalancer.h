#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <dirent.h>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

#define TRAIN_FILE_NAME  "train.csv"
#define WEUGHT_FILE_NAME "weights.csv"

class LoadBalancer
{
  public:
	LoadBalancer(string _dirname);
	~LoadBalancer();
	void read_trains_file();
	void read_weights_file();
	void set_extermum();
	void normalize();
	int calc_price_range(int index);
	void classification();
	void compute_accuracy();
	float dot_product(int weight_index, int data_index);

  private:
	float accuracy;
	string dirName;
	string train_file_name;
	string weight_file_name;

	vector< float > maximum;
	vector< float > minimum;
	vector< vector< float >> data;
	vector< vector< float >> weights;
};