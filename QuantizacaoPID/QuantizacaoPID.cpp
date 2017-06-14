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
auto total_sum = 0;

void print_vector(vector<UV_channel> vector)
{
	for (auto it = vector.begin(); it != vector.end(); ++it)
	{
		cout << it->u << "\t" << it->v << "\t F = " << it->freq << endl;
	}
}

void set_median(vector<UV_channel> uv_vector, int total_sum)
{
	// auxiliary file to see medians, frequences
	ofstream data_file;

	ofstream new_file;

	data_file.open("app_data.txt", ios::app);
	new_file.open("new_file.txt");

	auto median = 0; 

	for(vector<UV_channel>::iterator it = uv_vector.begin(); it != uv_vector.end(); ++it)
	{
		if (median < total_sum/2)
		{
			median += it->freq;
			data_file << "a\t" << it->u << "\t" << it->v << "\t" << it->freq << "\tSA = " << median << endl;
		} else
		{
			median += it->freq;
			data_file << "b\t" << it->u << "\t" << it->v << "\t" << it->freq << "\tSA = " << median << endl;
		}
	}

	data_file << "-------------------------------------------------------------------" << endl << endl;

	data_file.close();
	new_file.close();
}

void sort_by (vector<UV_channel> uv_vector, char channel)
{
	ofstream sorted_channel;
	channel == 'u' ? sorted_channel.open("sort_by_u.txt", ios::app) : sorted_channel.open("sort_by_v.txt", ios::app);

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

	sorted_channel.close();
}

void set_range(vector<UV_channel> uv_vector)
{
	auto u_min_max = minmax_element(uv_vector.begin(), uv_vector.end(), [](const UV_channel &a, const UV_channel &b)
	{
		return a.u < b.u;
	});
	auto v_min_max = minmax_element(uv_vector.begin(), uv_vector.end(), [](const UV_channel &a, const UV_channel &b)
	{
		return a.v < b.v;
	});

	(u_min_max.second->u - u_min_max.first->u) > (v_min_max.second->v - v_min_max.first->v) ? sort_by(uv_vector, 'u') : sort_by(uv_vector, 'v');
}

int main()
{
	ifstream colors(FILE_NAME);

	int u;
	int v;
	int freq;
	UV_channel hist;

	while (colors >> u)
	{
		colors >> v;
		colors >> freq;
		hist.u = u;
		hist.v = v;
		hist.freq = freq;
		uv_vector.push_back(hist);
	}

	colors.close();

	set_range(uv_vector);
	set_median(uv_vector, total_sum);

    return 0;
}
