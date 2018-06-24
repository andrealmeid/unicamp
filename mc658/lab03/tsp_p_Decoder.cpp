/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#ifndef TSP_P_DECODER_H
#define TSP_P_DECODER_H

#include <list>
#include <vector>
#include <algorithm>
#include "tsp_p.h"


class tsp_p_Decoder {
public:
	tsp_p_Decoder(const Tsp_P_Instance& instance);
	~tsp_p_Decoder();

	double decode(const std::vector< double >& chromosome) const;

private:
	const Tsp_P_Instance& instance;
};

#endif

bool comparator (pair<int, double> i, pair<int, double> j);

double pathCost(const Tsp_P_Instance &l, vector<int> path);

tsp_p_Decoder::tsp_p_Decoder(const Tsp_P_Instance& _instance) : instance(_instance) { }

tsp_p_Decoder::~tsp_p_Decoder() { }


//implement here your decoder
double tsp_p_Decoder::decode(const std::vector< double >& chromosome) const {

    // creates a tour: a pair(vertex index, chromosome value)
	vector<pair<int, double>> tour(chromosome.size());

	unsigned depot_id = instance.g.id(instance.depot);

	// fill vector with vertexes ordered by index
	for(unsigned i = 0, j = 0; i < chromosome.size(); i++, j++) {
		if(i == depot_id)
			j++;
		tour[i] = make_pair(j, chromosome[i]);
	}

    // sort the tour by chromosome
	sort(tour.begin(), tour.end(), comparator);

	vector<int> path(chromosome.size()+1);

    // fills the path with BRKGA's output
	for(unsigned i = 0; i < chromosome.size(); i++)
		path[i+1] = tour[i].first;

    path[0] = instance.g.id(instance.depot);

	return pathCost(instance, path);
}
