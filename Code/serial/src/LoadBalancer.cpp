#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(string _dirName)
{
	dirName = _dirName;
	train_file_name = dirName + TRAIN_FILE_NAME;
	weight_file_name = dirName + WEUGHT_FILE_NAME;
	this->read_trains_file();
	this->read_weights_file();
}

void LoadBalancer::read_trains_file()
{
	string line, field, firstLine;
	vector < float > gettingLine;
	
	ifstream myfile(train_file_name.c_str());
	
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
			data.push_back(gettingLine); 
		}
	}
}

void LoadBalancer::read_weights_file()
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

void LoadBalancer::set_extermum()
{
	for (int i = 0; i < data[0].size() - 1; ++i)
	{
		maximum.push_back(data[0][i]);
		minimum.push_back(data[0][i]);
	}

	for (int i = 1; i < data.size(); ++i)
	{
		for (int j = 0; j < data[i].size() - 1; ++j)
		{
			if (data[i][j] > maximum[j])
			{
				maximum[j] = data[i][j]; 
			}

			if (data[i][j] < minimum[j])
			{
				minimum[j] = data[i][j]; 
			}
		}
	}

	//cout << maximum.size() << endl;
}

void LoadBalancer::normalize()
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[i].size() - 1; ++j)
		{
			if ((maximum[j]  - minimum[j]) != 0)
			{
				data[i][j] = (data[i][j]  - minimum[j]) / (maximum[j]  - minimum[j]);
			}

			else
			{
				cout << "Normalizing Failed!" << endl;
			}
		}
	}

}

float LoadBalancer::dot_product(int weight_index, int data_index)
{
	float my_sum = 0;

	for (int i = 0; i < (weights[weight_index].size() - 1); i++) 
	{
		my_sum += (data[data_index][i] * weights[weight_index][i]);
	}

	return my_sum;
}


int LoadBalancer::calc_price_range(int index)
{
	vector <float> helper;

	for (int i = 0; i < weights.size(); ++i)
	{ 
		float temp = dot_product(i, index) + weights[i][weights[i].size() - 1];
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

void LoadBalancer::classification()
{
	for (int i = 0; i < data.size(); ++i)
	{
		data[i].push_back(calc_price_range(i));	
	}
}

void LoadBalancer::compute_accuracy()
{	
	int count = 0;

	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i][data[i].size() - 1] == data[i][data[i].size() - 2])
		{
			count++;
		}
	}

	accuracy = ((float)count) / ((float)data.size())*100;
	accuracy = floor(100*accuracy)/100;

	printf("Accuracy: %.2f%c\n", accuracy, '%');
}

LoadBalancer::~LoadBalancer()
{
}