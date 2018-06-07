/*
 * SampleDecoder.cpp
 *
 *  Created on: Jan 14, 2011
 *      Author: rtoso
 */

#include "tsp_p_Decoder.h"

tsp_p_Decoder::tsp_p_Decoder(const Tsp_P_Instance& _instance) : instance(_instance) { }

tsp_p_Decoder::~tsp_p_Decoder() { }

double pathCost2(const Tsp_P_Instance &l, vector<int> path){

    double calcCost = 0.0;
    double sum_ta=0.0;

    lemon::ListDigraphBase::Node n, m;

    int tour_size = path.size();
    for(int i = 0; i < tour_size-1; i++){

        n = l.g.nodeFromId(path[i]);
        m = l.g.nodeFromId(path[i+1]);

        Arc a = findArc(l.g, n, m);

        sum_ta += l.weight[a];
        calcCost += l.weight_node[m] * sum_ta;
    }

    return calcCost;
}

bool myfunction (pair<int, double> i, pair<int, double> j) {
	return (i.second < j.second);
}

//implement here your decoder
double tsp_p_Decoder::decode(const std::vector< double >& chromosome) const {

	vector<pair<int, double>> tour(chromosome.size());

	for(unsigned i = 0; i < chromosome.size(); i++) {
		 tour[i] = make_pair(i+1, chromosome[i]);
	 }

	std::sort(tour.begin(), tour.end(), myfunction);

	vector<int> path(chromosome.size()+1);

	for(unsigned i = 0; i < chromosome.size(); i++)
		path[i+1] = tour[i].first;

    path[0] = instance.g.id(instance.depot);

    for(unsigned i = 0; i < path.size(); i++)
        cout << path[i] << " ";
    cout << endl;

	return pathCost2(instance, path);
}
