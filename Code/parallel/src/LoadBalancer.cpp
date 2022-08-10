#include "LoadBalancer.h"

int total_size;

string weight_file_name;

vector< float > maximum;
vector< float > minimum;
vector <Input> thread_detail;
vector <vector <float>> weights;
vector <vector <vector <float>>> data(NUMBER_OF_THREADS);

Input::Input()
{

}

void load_balancer(string datasets_directory)
{
	weight_file_name = datasets_directory + WEIGHT_FILE_NAME;

	read_weights_file();
	make_ready(datasets_directory);
}

void read_weights_file()
{
	string line, field, firstLine; 
	vector< float > gettingLine;
	
	ifstream myfile(weight_file_name.c_str());
	
	if(myfile.is_open())
	{
		getline(myfile, firstLine);
		while (getline(myfile,line))
		{
			gettingLine.clear();
			stringstream ss(line);
			while(getline(ss,field,','))
			{
				gettingLine.push_back(stof(field));
			}
			weights.push_back(gettingLine);
		}
	}
}

void make_ready(string dirName)
{
	void* status;

	int return_code;
	init_thread_details(dirName);
	
	pthread_t thread[NUMBER_OF_THREADS];

	for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_create(&thread[tid], NULL, read_csv, (void*)&thread_detail[tid]);
	}

	for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		return_code = pthread_join(thread[tid], &status);

		if (return_code)
		{
			printf("ERROR; return code from pthread_join() is %d\n", return_code);
			exit(-1);
		}
	}
}

void init_thread_details(string dirName)
{
	for (long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		Input input;
		input.thread_number = tid;
		input.file_name = dirName + TRAIN + to_string(tid) + CSV;
		thread_detail.push_back(input);
	}	
}

void* read_csv(void* arg)
{
	Input* input = (Input*)arg;
	string file_name = input->file_name;
	string line, field, firstLine;
	
	vector <float> gettingLine;
	vector <vector <float>> local_data;

	ifstream myfile(file_name.c_str());
	
	if(myfile.is_open())
	{
		getline(myfile, firstLine);
		while (getline(myfile,line))
		{
			gettingLine.clear();
			stringstream ss(line);
			while(getline(ss,field,','))
			{
				gettingLine.push_back(stof(field));
			}

			local_data.push_back(gettingLine);
		}
	}
	else
	{
		cout << "Opening file fails!" << endl;
	}

    myfile.close();

	data[input->thread_number] = local_data;

    pthread_exit(NULL);
}

void set_extermum()
{
	for (int i = 0; i < NUMBER_OF_THREADS; ++i)
	{
		total_size += data[i].size();
	}

	for (int i = 0; i < data[0][0].size() - 1; ++i)
	{
		maximum.push_back(data[0][0][i]);
		minimum.push_back(data[0][0][i]);
	}

	for (int k = 0; k < NUMBER_OF_THREADS; ++k)
	{
		for (int i = 1; i < data[k].size(); ++i)
		{
			for (int j = 0; j < data[k][i].size() - 1; ++j)
			{
				if (data[k][i][j] > maximum[j])
				{
					maximum[j] = data[k][i][j]; 
				}

				if (data[k][i][j] < minimum[j])
				{
					minimum[j] = data[k][i][j]; 
				}
			}
		}
	}
}

void normalize()
{
	for (int k = 0; k < NUMBER_OF_THREADS; ++k)
	{
		for (int i = 0; i < data[k].size(); ++i)
		{
			for (int j = 0; j < data[k][i].size() - 1; ++j)
			{

				if ((maximum[j]  - minimum[j]) != 0)
				{
					data[k][i][j] = (data[k][i][j]  - minimum[j]) / (maximum[j]  - minimum[j]);
				}

				else
				{
					cout << "Normalizing Failed!" << endl;
				}
			}
		}
	}
}

float dot_product(int weight_index, int data_index, long tid)
{
	float my_sum = 0;

	for (int i = 0; i < (weights[weight_index].size() - 1); i++) 
	{
		my_sum += (data[tid][data_index][i] * weights[weight_index][i]);
	}

	return my_sum;
}


int calc_price_range(int index, long tid)
{
	vector <float> helper;

	for (int i = 0; i < weights.size(); ++i)
	{ 
		float temp = dot_product(i, index, tid) + weights[i][weights[i].size() - 1];
		helper.push_back(temp);
	}

	int trained_price_index = 0;
	float trained_price = helper[0];

	for (int i = 1; i < helper.size(); ++i)
	{
		if (helper[i] > trained_price)
		{
			trained_price = helper[i];
			trained_price_index = i;
		}
	}

	return trained_price_index;
}

void classification()
{
	void* status;
	int return_code;

	pthread_t thread[NUMBER_OF_THREADS];
	
	for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		pthread_create(&thread[tid], NULL, classification_parallel, (void*)&thread_detail[tid]);
	}

	for(long tid = 0; tid < NUMBER_OF_THREADS; tid++)
	{
		return_code = pthread_join(thread[tid], &status);

		if (return_code)
		{
			printf("ERROR; return code from pthread_join() is %d\n", return_code);
			exit(-1);
		}
	}
}

void* classification_parallel(void* arg)
{
	Input* input = (Input*)arg;
	long tid = input->thread_number;
	
	for (long i = 0; i < data[tid].size(); ++i)
	{
		data[tid][i].push_back(calc_price_range(i, tid));	
	}

    pthread_exit(NULL);
}

void compute_accuracy()
{	
	int count = 0;

	for (int k = 0; k < NUMBER_OF_THREADS; ++k)
	{
		for (int i = 0; i < data[k].size(); ++i)
		{
			if (data[k][i][data[k][i].size() - 1] == data[k][i][data[k][i].size() - 2])
			{
				count++;
			}
		}
	}

	float accuracy = ((float)count) / ((float)total_size)*100;
	accuracy = floor(100*accuracy)/100;
	printf("Accuracy: %.2f%c\n", accuracy, '%');
}