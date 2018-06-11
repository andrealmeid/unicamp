/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "tsp_p_Decoder.h"

bool comparator (pair<int, double> i, pair<int, double> j); 

double pathCost(const Tsp_P_Instance &l, vector<int> path);

tsp_p_Decoder::tsp_p_Decoder(const Tsp_P_Instance& _instance) : instance(_instance) { }

tsp_p_Decoder::~tsp_p_Decoder() { }


//implement here your decoder
double tsp_p_Decoder::decode(const std::vector< double >& chromosome) const {

	vector<pair<int, double>> tour(chromosome.size());

	for(unsigned i = 0; i < chromosome.size(); i++) {
		 tour[i] = make_pair(i+1, chromosome[i]);
	 }

	std::sort(tour.begin(), tour.end(), comparator);

	vector<int> path(chromosome.size()+1);

	for(unsigned i = 0; i < chromosome.size(); i++)
		path[i+1] = tour[i].first;

    path[0] = instance.g.id(instance.depot);

    for(unsigned i = 0; i < path.size(); i++)
        cout << path[i] << " ";
    cout << endl;

	return pathCost(instance, path);
}
