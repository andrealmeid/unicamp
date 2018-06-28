/*******************************************************************************
* VERSION: 1.0
* MC658 - Projeto e Análise de Algoritmos III - 1s2018
* Prof: Flavio Keidi Miyazawa
* PED: Francisco Jhonatas Melo da Silva
* Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa
******************************************************************************/

/* IMPLEMENTE AS FUNCOES INDICADAS
* DIGITE SEU RA: 164047
* SUBMETA SOMENTE ESTE ARQUIVO */

// System libs
#include <iostream>
#include <float.h>
#include <math.h>
#include <lemon/list_graph.h>
#include <algorithm>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <ctime>

// Project libs
#include "mygraphlib.h"
#include "tsp_p_algs.h"
#include "MTRand.h"
#include "BRKGA.h"
#include "gurobi_c++.h"

// flag to print debug outputs
#define DEBUG 0

// BRKGA decoder class to TSP-P problem
class tsp_p_Decoder {
public:
	tsp_p_Decoder(const Tsp_P_Instance& instance);
	~tsp_p_Decoder();

	double decode(const std::vector< double >& chromosome) const;

private:
	const Tsp_P_Instance& instance;
};

// function to check if the solution found by gurobi has a subtour
void findsubtour(int n, double** sol, int* tourlenP, int* tour);

// class to be used with Gurobi's callback to find a subtour
// credits: www.gurobi.com/documentation/7.5/examples/tsp_cpp_cpp.html
class subtourelim: public GRBCallback
{
public:
	GRBVar** vars;
	int n;
	subtourelim(GRBVar** xvars, int xn) {
		vars = xvars;
		n    = xn;
	}
protected:
	void callback() {
		try {
			if (where == GRB_CB_MIPSOL) {
				// Found an integer feasible solution - does it visit every node?
				double **x = new double*[n];
				int *tour = new int[n];
				int i, j, len;
				for (i = 0; i < n; i++)
				x[i] = getSolution(vars[i], n);

				findsubtour(n, x, &len, tour);

				if (len < n) {
					// Add subtour elimination constraint
					GRBLinExpr expr = 0;
					for (i = 0; i < len; i++)
					for (j = i+1; j < len; j++)
					expr += vars[tour[i]][tour[j]];
					addLazy(expr <= len-1);
				}

				for (i = 0; i < n; i++)
				delete[] x[i];
				delete[] x;
				delete[] tour;
			}
		} catch (GRBException e) {
			cout << "Error number: " << e.getErrorCode() << endl;
			cout << e.getMessage() << endl;
		} catch (...) {
			cout << "Error during callback" << endl;
		}
	}
};

// alarm functions
volatile sig_atomic_t timeout = 0;

static void alarm_handler(int sig);

// find the cost of a given path
double pathCost(const Tsp_P_Instance &l, vector<int> path);

// get a random neighbor of a tour
vector<int> getRandomNeighbor(vector<int> v, int tour_size);

// prints the vector
void printVector(vector<int> v){
	for(int i = 0; i < (int)v.size(); i++)
	cout << v[i] << " ";
	cout << endl;
}

// compartor to compare a pair chromosome-vector
bool comparator (pair<int, double> i, pair<int, double> j);

// swaps path in the interval [j, k]
vector<int> opt2Swap(vector<int> path, int j,  int k);

// 2-OPT implementation to TSP
vector<int> opt2(vector<int> path, const Tsp_P_Instance &l);

//------------------------------------------------------------------------------
bool constrHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
	/*
	A quick constructive heuristic implementation for TSP-P
	*/

	// initiate alarm
	signal(SIGALRM, alarm_handler);
	alarm(tl);

	// Initialize solution
	s.tour.clear();
	s.cost = 0.0;
	DNode depot;
	depot = l.depot; //tour begins at s
	s.tour.push_back(depot);
	OutArcIt arc_min;

	// Gets input information
	int depot_id = l.g.id(depot);
	int tour_size = l.n;


	// creates a vector with ids of the unused vertices on the path
	vector<int> avalibleVertices(tour_size-1);
	for(int i = 0, j = 0; i < tour_size-1; i++){
		if(j == depot_id)
			j++;
		avalibleVertices[i] = j++;
	}

	// initialize vector with choosen ids
	vector<int> path;
	path.push_back(depot_id);

	DNode source, target;
	// auxiliar variables
	int lowest_cost = (int) DBL_MAX;
	int lowest_id = 0;
	int id_to_remove = 0;
	int sum_ta = 0;

	// gets the insertion with the lowest impact on cost
	while(!avalibleVertices.empty()){
		if (timeout) break;
		lowest_cost = (int) DBL_MAX;

		// get the last vertice on the path
		int source_id = path.back();
		source = l.g.nodeFromId(source_id);

		// checks all remain vertices on the graph
		for(unsigned i = 0; i < avalibleVertices.size(); i++){
			int target_id = avalibleVertices[i];

			target = l.g.nodeFromId(target_id);
			Arc a = findArc(l.g, source, target);

			int arc_peso =  l.weight[a];
			int node_peso = l.weight_node[target];

			int cost = arc_peso * node_peso;

			if(cost < lowest_cost){
				lowest_cost = cost;
				lowest_id = target_id;
				id_to_remove = i;
			}
		}

		target = l.g.nodeFromId(lowest_id);
		Arc min_arc = findArc(l.g, source, target);
		int min_arc_peso =  l.weight[min_arc];
		int min_node_peso = l.weight_node[target];
		sum_ta += min_arc_peso;
		s.cost += min_node_peso * sum_ta;

		// add vertice on the path
		path.push_back(lowest_id);
		s.tour.push_back(target);

		// removes taken vertice from avalible vertices
		for(unsigned i = id_to_remove; i < avalibleVertices.size(); i++){
			avalibleVertices[i] = avalibleVertices[i+1];
		}
		avalibleVertices.pop_back();
	}

	return false;
}

//------------------------------------------------------------------------------//

/* if META_2OPT, meta-heuristic will use 2-OPT to find a neighbor
   else, it will use getRandomNeighbor function */
#define META_2OPT false

#define INITAL_TEMPERATURE 2000
#define MIN_TEMPERATURE 10
#define INNER_LIMIT 300 // inner loop limit
#define ALPHA_PARAM 0.96 // alpha parameter of geometric decay
#define K_PARAM 2 // k parameter that multiplies temperature

bool metaHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
	/*
	simulated annealing implementation for TSP-P
	*/

	// initizalize alarm
	signal(SIGALRM, alarm_handler);
	alarm(tl);

	// initizalize simulated annealing variables
	double temperature = INITAL_TEMPERATURE;
	const long unsigned rngSeed = time(0);
	MTRand rng(rngSeed);

	// instance variables
	int tour_size = l.n;
	int depot_id = l.g.id(l.depot);

	// a vector with the index of the current path
	vector<int> current_path(tour_size);
	current_path[0] = depot_id;

	// find a initial path using constructive heuristic
	Tsp_P_Solution constrSolution;
	constrHeur(l, constrSolution, tl);

	// initial path from constrHeur
	for(int i = 1; i < tour_size; i++){
		current_path[i] = l.g.id(constrSolution.tour[i]);
	}

	int current_value = pathCost(l, current_path);

	// variables to store global best values
	int best_value = current_value;
	vector<int> best_path = current_path;

	// while temperature hasnt achieved the minimal temperature
	while(temperature > MIN_TEMPERATURE){
		// while it hasnt achieved the inner_limit or hasnt a new best global
		for(int i = 0; i < INNER_LIMIT; i++){
			// time's up, return current best solution
			if (timeout) {
				temperature = MIN_TEMPERATURE;
				break;
			}

			// neighbor solution = 2-OPT or random neighbor
			#if META_2OPT
			vector<int> neighbor_path = opt2(current_path, l);
			#else
			vector<int> neighbor_path = getRandomNeighbor(current_path, tour_size - 1);
			#endif

			int neighbor_value = pathCost(l, neighbor_path);

			double diff = neighbor_value - current_value;

			// verifies the difference between results
			if (diff <= 0){
				// updates current solution
				current_path = neighbor_path;
				current_value = neighbor_value;

				// updates global solution
				if (neighbor_value < best_value){
					best_value = neighbor_value;
					best_path = neighbor_path;
					break;
				}

			} else {
				// calculates random probability to use worse solution
				double random_value = rng.rand();
				double param = -diff/(temperature * K_PARAM);
				double e = exp(param);

				// gets the new value
				if (e > random_value){
					current_path = neighbor_path;
					current_value = neighbor_value;
				}

			}
		}
		// temperature's geometric decay
		temperature = temperature * ALPHA_PARAM;
	}

	// set's current best global solution as solution
	s.cost = best_value;

	s.tour.clear();
	s.tour.push_back(l.depot);

	for(int i = 1; i < tour_size; i++){
		DNode node = l.g.nodeFromId(best_path[i]);
		s.tour.push_back(node);
	}

	return false;
}

//------------------------------------------------------------------------------
/* BRKGA constants */
#define P 256 // size of population
#define PE 0.2 // fraction of population to be the elite-set
#define PM 0.20 // fraction of population to be replaced by mutants
#define RHOE 0.70 // probability that offspring inherit an allele from elite parent
#define K 3 // number of independent populations
#define MAXT 2 // number of threads for parallel decoding

/* BRKGA inner loop (evolution) configuration: Exchange top individuals */
#define X_INTVL 100 // exchange best individuals at every 100 generations
#define X_NUMBER 2 // exchange top 2 best
#define MAX_GENS 300 // run for 300 generations

/* BRKGA evolution configuration: restart strategy */
#define RESET_AFTER 200

bool brkga(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
	/*
	BRKGA TSP-P implementation, mostly based on:
	https://github.com/rfrancotoso/brkgaAPI/tree/master/examples/brkga-tsp
	by Rodrigo Franco Toso and Mauricio G.C. Resende
	*/

	// initizalize alarm
	signal(SIGALRM, alarm_handler);
	alarm(tl);

	// Initialize decoder
	tsp_p_Decoder decoder(l);

	// Initialize random number generator
	const long unsigned rngSeed = time(0);
	MTRand rng(rngSeed);

	// size of chromosomes
	const unsigned tour_size = l.n-1;

	BRKGA< tsp_p_Decoder, MTRand > algorithm(tour_size, P, PE, PM, RHOE, decoder, rng, K, MAXT);

	// last relevant generation: best updated or reset called
	unsigned relevantGeneration = 1;

	// global variables to store best global solution
	vector< double > bestChromosome;
	double bestFitness = numeric_limits< double >::max();

	// Run the evolution loop:
	for(unsigned generation = 1; generation < MAX_GENS; generation++){
		if (timeout) break; // time's up! return current solution

		algorithm.evolve(); // evolve the population for one generation

		// Bookeeping: has the best solution thus far improved?
		if(algorithm.getBestFitness() < bestFitness) {
			// Save the best solution to be used after the evolution chain:
			relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();
		}

		//  Evolution strategy: restart
		if(generation - relevantGeneration > RESET_AFTER) {
			algorithm.reset();  // restart the algorithm with random keys
			relevantGeneration = generation;
		}

		// Evolution strategy: exchange top individuals among the populations
		if(generation % X_INTVL == 0 && relevantGeneration != generation)
			algorithm.exchangeElite(X_NUMBER);
	}

	// rebuild the best solution from the best chromosome
	s.tour.clear();
	s.cost = bestFitness;

	DNode depot;
	depot = l.depot; //tour begins at s
	int depot_id = l.g.id(depot);

	// creates a tour: a pair(vertex index, chromosome value)
	vector<pair<int, double>> tour(tour_size);

	// fill vector with vertexes ordered by index
	for(unsigned i = 0, j = 0; i < tour_size; i++, j++) {
		if((int) i == depot_id)
		j++;
		tour[i] = make_pair(j, bestChromosome[i]);
	}

	// sort the tour by chromosome
	sort(tour.begin(), tour.end(), comparator);

	// tour begins at depot
	s.tour.push_back(depot);

	// fills the tour with BRKGA's output
	for(unsigned i = 0; i < tour_size; i++){
		DNode node = l.g.nodeFromId(tour[i].first);
		s.tour.push_back(node);
	}

	return false;
}
//------------------------------------------------------------------------------
bool exact(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{

	/*
	PLI implementation to TSP-P, mostly based on:
	http://www.gurobi.com/documentation/7.5/examples/tsp_cpp_cpp.html
	*/

	try {

		int n = l.n;	// number of vectors

		// define Gurobi's environment and model
		GRBEnv *env = NULL;
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);

		// set to use lazy constraints
		model.set(GRB_IntParam_LazyConstraints, 1);

		// turn off Gurobi verbose output
		model.set(GRB_IntParam_OutputFlag, 0);

		model.getEnv().set(GRB_IntParam_Threads, 1);

		// calculates heuristic time spent
		time_t heur_time_a = time(nullptr);

		// define cutoff value from heuristic solution
		Tsp_P_Solution heuristic_solution;
		constrHeur(l, heuristic_solution, tl/0.2);
		model.getEnv().set(GRB_DoubleParam_Cutoff, heuristic_solution.cost);

		// remove the time spent by heuristic from the gurobi time
		time_t heur_time_b = time(nullptr);

		// update the timelimit
		tl -= heur_time_b - heur_time_a;

		// bound the execution time
		model.getEnv().set(GRB_DoubleParam_TimeLimit, tl);

		// x_ij: a binary var to set if a arc is or isn't on the solution
		GRBVar **x = NULL;

		x = new GRBVar*[n];
		for (int i = 0; i < n; i++)
			x[i] = new GRBVar[n];

		// time of each vector
		GRBVar *time = new GRBVar[n];

		// calculates the sum of all times
		// find most heaviest arc
		double total_time = 0;
		double biggest_time = 0;
		for(ListDigraph::ArcIt arc(l.g); arc != INVALID; ++arc){
			double current_time = l.weight[arc];
			if(current_time > biggest_time)
			biggest_time = current_time;
		}

		total_time = biggest_time * n;

		// array to store the nodes indexes according to Lemon order
		vector<DNode> nodes(n);
		for(ListDigraph::NodeIt node(l.g); node != INVALID; ++node){
			nodes[l.g.id(node)] = node;
		}

		// time of each node
		for(int i = 0; i < n; i++){
			#if DEBUG
			cout << "id: " << i << " peso: " << l.weight_node[nodes[i]] << endl;
			#endif

			time[i] = model.addVar(0.0, total_time, l.weight_node[nodes[i]], GRB_CONTINUOUS, "t");
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				x[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "x");

		// sum x_ij = 2
		for (int i = 0; i < n; i++) {
			GRBLinExpr expr_in = 0;
			GRBLinExpr expr_out = 0;
			for (int j = 0; j < n; j++){
				expr_in += x[i][j];
				expr_out += x[j][i];
			}
			model.addConstr(expr_out == 1);
			model.addConstr(expr_in == 1);
		}

		// depot should always be the first vertex
		model.addConstr(time[l.g.id(l.depot)] == 0);
		#if DEBUG
		cout << "depot id: " << l.g.id(l.depot) << endl;
		#endif

		for (int i = 0; i < n; i++){
			if (i == l.g.id(l.depot)) continue;
			for (int j = 0; j < n; j++){

				double value = l.weight[findArc(l.g, nodes[j], nodes[i])];
				model.addConstr(
					time[i] >= time[j] + value - (1 - x[j][i]) * total_time
				);
			}
		}

		// sum x_ii = 0
		for (int i = 0; i < n; i++)
		x[i][i].set(GRB_DoubleAttr_UB, 0);

		// defines callback routine
		subtourelim cb = subtourelim(x, n);
		model.setCallback(&cb);

		model.update();
		model.optimize();

		// extract the path from the solution
		if (model.get(GRB_IntAttr_SolCount) > 0) {
			double **sol = new double*[n];
			for (int i = 0; i < n; i++)
				sol[i] = model.get(GRB_DoubleAttr_X, x[i], n);

			int* tour = new int[n];
			int len;

			vector<int> tour_indexes(n);

			findsubtour(n, sol, &len, tour);

			#if DEBUG
			if(len < n) cout << "eita" << endl;
			#endif

			for (int i = 0; i < len; i++){
				tour_indexes[i] = tour[i];
				s.tour.push_back(nodes[tour[i]]);
			}

			int cost = pathCost(l, tour_indexes);
			s.cost = cost;

			for (int i = 0; i < n; i++)
				delete[] sol[i];
			delete[] sol;
			delete[] tour;

			// if time's up, return false
			if (model.get(GRB_IntAttr_Status) == GRB_TIME_LIMIT)
				return false;

		}

	} catch (GRBException e) {
		cout << "Error number: " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch (...) {
		cout << "Error during optimization" << endl;
	}

	return true;
}
//------------------------------------------------------------------------------

// alarm function
static void alarm_handler(int sig)
{
	timeout = 1;
}

// find the cost of a given path
double pathCost(const Tsp_P_Instance &l, vector<int> path){

	double calcCost = 0.0;
	double sum_ta = 0.0;

	DNode n, m;

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

// get a random neighbor of a tour
vector<int> getRandomNeighbor(vector<int> r, int tour_size){
	int i = (rand() % tour_size);
	int j = (rand() % tour_size);

	while(i == j){
		j = (rand() % tour_size);
	}

	i++; j++;

	int aux = r[i];
	r[i] = r[j];
	r[j] = aux;

	return r;
}

// compartor to compare a pair chromosome-vector
bool comparator (pair<int, double> i, pair<int, double> j) {
	return (i.second < j.second);
}

// swaps path in the interval [j, k]
vector<int> opt2Swap(vector<int> r, int j,  int k){
	if ((j > k) || k >= (int) r.size())
		return r;

	for(int i = 0; i < ceil((k-j)/2.0); i++){
		int aux = r[j+i];
		r[j+i] = r[k-i];
		r[k-i] = aux;
	}

	return r;

}

// 2-OPT implementation to TSP
vector<int> opt2(vector<int> path, const Tsp_P_Instance &l){
	int best_value = pathCost(l, path);
	int tour_size = path.size();
	bool improved = true;

	while(improved){
		improved = false;
		for(int i = 1; i < tour_size - 1; i++){
			for(int j = i + 1; j < tour_size; j++){
				vector<int>aux = opt2Swap(path, i, j);
				int local_cost = pathCost(l, aux);
				if (local_cost < best_value){
					best_value = local_cost;
					path = aux;
					improved = true;
					break;
				}
			}
		}
	}

	return path;
}

// function to check if the solution found by gurobi has a subtour
void findsubtour(int n, double** sol, int* tourlenP, int* tour)
{
	// credits: www.gurobi.com/documentation/7.5/examples/tsp_cpp_cpp.html
	bool* seen = new bool[n];
	int bestind, bestlen;
	int i, node, len, start;

	for (i = 0; i < n; i++)
	seen[i] = false;

	start = 0;
	bestlen = n+1;
	bestind = -1;
	node = 0;
	while (start < n) {
		for (node = 0; node < n; node++)
		if (!seen[node])
		break;
		if (node == n)
		break;
		for (len = 0; len < n; len++) {
			tour[start+len] = node;
			seen[node] = true;
			for (i = 0; i < n; i++) {
				if (sol[node][i] > 0.5 && !seen[i]) {
					node = i;
					break;
				}
			}
			if (i == n) {
				len++;
				if (len < bestlen) {
					bestlen = len;
					bestind = start;
				}
				start += len;
				break;
			}
		}
	}

	for (i = 0; i < bestlen; i++)
	tour[i] = tour[bestind+i];
	*tourlenP = bestlen;

	delete[] seen;
}
