// QuantizacaoPID.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>

#define FILE_NAME "new_file.txt"

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

void set_median(vector<UV_channel> uv_vector, int total_sum)
{
	// auxiliary file to see medians, frequences
	ofstream data_file;

	ofstream new_file;

	data_file.open("app_data.txt", ios::app);

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
}

void sort_by (vector<UV_channel> uv_vector, char channel)
{
	ofstream sorted_channel;
	auto total_sum = 0;
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

	set_median(uv_vector, total_sum);
	sorted_channel.close();
}

void set_range(vector<UV_channel> uv_vector)
{
	decltype(uv_vector)::iterator minU, maxU;
	decltype(uv_vector)::iterator minV, maxV;

	tie(minU, maxU) = minmax_element(begin(uv_vector), end(uv_vector), [](UV_channel const& a, UV_channel const& b)
	{
		return a.u < b.u;
	});
	//auto u_min_max = minmax_element(uv_vector.begin(), uv_vector.end(), [](const UV_channel &a, const UV_channel &b)
	//{
	//	return a.u < b.u;
	//});
	tie(minV, maxV) = minmax_element(begin(uv_vector), end(uv_vector), [](UV_channel const& a, UV_channel const& b)
	{
		return a.v < b.v;
	});

	cout << minU->u << " " << maxU->u << endl;
	cout << minV->v << " " << maxV->v << endl;
	(maxU->u - minU->u) > (maxV->v - minV->v) ? sort_by(uv_vector, 'u') : sort_by(uv_vector, 'v');
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

    return 0;
}
