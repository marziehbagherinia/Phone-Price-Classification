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
#include <stdexcept> // std::runtime_error
#include <pthread.h>
#include <map>


using namespace std;
using namespace std::chrono; 

#define ZERO  0
#define ONE   1
#define TWO   2
#define THREE 3
#define NUMBER_OF_THREADS 8

#define CSV    ".csv"
#define TRAIN  "train_"
#define WEIGHT_FILE_NAME "weights.csv"

class Input
{
  public:
	Input();
	long thread_number;
	string file_name;
};

void* read_csv(void* arg);
void* classification_parallel(void* arg);

void normalize();
void set_extermum();
void classification();
void compute_accuracy();
void read_weights_file();
void make_ready(string dirName);
void init_thread_details(string dirName);
void load_balancer(string datasets_directory);

int calc_price_range(int index, long tid);
float dot_product(int weight_index, int data_index, long tid);