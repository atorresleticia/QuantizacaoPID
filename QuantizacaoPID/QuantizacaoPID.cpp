// QuantizacaoPID.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <queue>

#define FILE_NAME "main_out.txt"

using namespace std;

typedef struct UV
{
	int u = 0;
	int v = 0;
	int freq = 0;
} UV_channel;

vector<UV_channel> uv_vector;

void print_vector(vector<UV_channel> vector)
{
	for (auto it = vector.begin(); it != vector.end(); ++it)
	{
		cout << it->u << "\t" << it->v << "\t F = " << it->freq << endl;
	}
}

void median_cut(vector<UV_channel> uv_vector, int total_sum)
{
	vector<UV_channel> a;
	vector<UV_channel> b;

	// auxiliary file to see medians, frequences
	ofstream aux_file;

	// super very important file to read
	ofstream pid_file;
	ofstream pid_file2;

	aux_file.open("auxiliary.csv", ios::app);
	pid_file.open("pid1.txt");
	pid_file2.open("pid2.txt");

	auto median = 0; 

	for(vector<UV_channel>::iterator it = uv_vector.begin(); it != uv_vector.end(); ++it)
	{
		if (median < total_sum/2)
		{
			median += it->freq;
			a.push_back(*it);
			aux_file << "a\t" << it->u << "\t" << it->v << "\t" << it->freq << "\tSA = " << median << endl;
			pid_file << it->u << "\t" << it->v << "\t" << it->freq << endl;
		} else
		{
			median += it->freq;
			b.push_back(*it);
			aux_file << "b\t" << it->u << "\t" << it->v << "\t" << it->freq << "\tSA = " << median << endl;
			pid_file2 << it->u << "\t" << it->v << "\t" << it->freq << endl;
		}
	}

	aux_file << "-------------------------------------------------------------------" << endl << endl;

	aux_file.close();
	pid_file.close();
	pid_file2.close();
}

void order_by (vector<UV_channel> uv_vector, char channel)
{
	ofstream sorted_channel;
	channel == 'u' ? sorted_channel.open("sort_by_u.txt", ios::app) : sorted_channel.open("sort_by_v.txt", ios::app);

	auto total_sum = 0;

	if(channel == 'u')
	{
		sort(uv_vector.begin(), uv_vector.end(), [](const UV_channel &a, const UV_channel &b)
		{
			return (a.u < b.u);
		});

		for (vector<UV_channel>::iterator it = uv_vector.begin(); it != uv_vector.end(); ++it)
		{
			total_sum += it->freq;
			sorted_channel << it->u << "\t" << it->v << "\tF = " << it->freq << "\tSA = " << total_sum << endl;
			cout << it->u << "\t" << it->v << "\tF = " << it->freq << "\tSA = " << total_sum << endl;
		}

		sorted_channel << "-------------------------------------------------------------------" << endl << endl;

	} else
	{
		sort(uv_vector.begin(), uv_vector.end(), [](const UV_channel &a, const UV_channel &b)
		{
			return (a.v < b.v);
		});

		for (vector<UV_channel>::iterator it = uv_vector.begin(); it != uv_vector.end(); ++it)
		{
			total_sum += it->freq;
			sorted_channel << it->u << "\t" << it->v << "\tF = " << it->freq << "\tSA = " << total_sum << endl;
			cout << it->u << "\t" << it->v << "\tF = " << it->freq << "\tSA = " << total_sum << endl;
		}

		sorted_channel << "-------------------------------------------------------------------" << endl << endl;
	}

	median_cut(uv_vector, total_sum);
	sorted_channel.close();
}

void quantization(vector<UV_channel> uv_vector)
{
	auto greatest_u = 0;
	auto greatest_v = 0;
	auto lowest_u = 255;
	auto lowest_v = 255;

	for (vector<UV_channel>::iterator it = uv_vector.begin(); it != uv_vector.end(); ++it)
	{
		if (greatest_u < it->u)
		{
			greatest_u = it->u;
		}
		if (greatest_v < it->v)
		{
			greatest_v = it->v;
		}
		if (lowest_u > it->u)
		{
			lowest_u = it->u;
		}
		if (lowest_v > it->v)
		{
			lowest_v = it->v;
		}
	}

	(greatest_u - lowest_u) > (greatest_v - lowest_v) ? order_by(uv_vector, 'u') : order_by(uv_vector, 'v');
}

int main()
{
	ifstream colors;
	colors.open(FILE_NAME);

	UV_channel color_hist;
	int u;
	int v;
	int freq;

	while (colors >> u)
	{
		colors >> v;
		colors >> freq;
		color_hist.u = u;
		color_hist.v = v;
		color_hist.freq = freq;
		uv_vector.push_back(color_hist);
	}
	
	quantization(uv_vector);

	colors.close();
    return 0;
}
