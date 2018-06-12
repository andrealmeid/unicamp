/*******************************************************************************
 ** VERSION: 1.0
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa
 ******************************************************************************/

/* IMPLEMENTE AS FUNCOES INDICADAS
 * DIGITE SEU RA: 164047
 * SUBMETA SOMENTE ESTE ARQUIVO */

#include <iostream>
#include <float.h>
#include <math.h>
#include <lemon/list_graph.h>
#include <algorithm>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <time.h>

#include "mygraphlib.h"
#include "tsp_p_algs.h"
#include "tsp_p_Decoder.h"
#include "MTRand.h"
#include "BRKGA.h"

#define DEBUG 1

// alarm functions
volatile sig_atomic_t timeout = 0;
static void alarm_handler(int sig);

static void alarm_handler(int sig)
{
 	timeout = 1;
}

bool naive(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);

double pathCost(const Tsp_P_Instance &l, vector<int> path);

vector<int> getSolucaoVizinha(vector<int> v, int tour_size);

void printVector(vector<int> v){
    for(int i = 0; i < (int)v.size(); i++)
        cout << v[i] << " ";
    cout << endl;
}

bool comparator (pair<int, double> i, pair<int, double> j);

// swaps path in the interval [j, k]
vector<int> opt2Swap(vector<int> path, int j,  int k);

vector<int> opt2(vector<int> path, const Tsp_P_Instance &l);

vector<int> getFirstSolution(int depot_id, int tour_size);

//------------------------------------------------------------------------------
bool constrHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    // initiate alarm
   	signal(SIGALRM, alarm_handler);
	alarm(tl);

    // Initialize solution
    s.tour.clear();
    s.cost = 0.0;
    DNode depot;
    depot = l.depot; //tour begins at s
    s.tour.push_back(depot);

    // // Gets input information
    // int depot_id = l.g.id(depot);
    // int tour_size = l.n;
    //
    OutArcIt arc_min;
    //
    // // creates a vector with ids of the unused vertices on the path
    // vector<int> avalibleVertices(tour_size-1);
    // for(int i = 0, j = 0; i < tour_size-1; i++){
    //     if(j == depot_id)
    //         j++;
    //     avalibleVertices[i] = j++;
    // }
    //
    // // initialize vector with choosen ids
    // vector<int> path;
    // path.push_back(depot_id);
    //
    // // auxiliar variables
    // int lowest_cost = (int) DBL_MAX;
    // int lowest_id = 0;
    // lemon::ListDigraphBase::Node source, target;
    // int id_to_remove = 0;
    // int sum_ta = 0;
    //
    //
    // // gets the insertion with the lowest impact on cost
    // while(!avalibleVertices.empty()){
    //     if (timeout) break;
    //     lowest_cost = (int) DBL_MAX;
    //
    //     #if DEBUG
    //     printVector(path);
    //     printVector(avalibleVertices);
    //     #endif
    //
    //     // get the last vertice on the path
    //     int source_id = path.back();
    //     source = l.g.nodeFromId(source_id);
    //
    //     #if DEBUG
    //     cout << "id = " << source_id << endl;
    //     #endif
    //
    //     // checks all remain vertices on the graph
    //     for(unsigned i = 0; i < avalibleVertices.size(); i++){
    //         int target_id = avalibleVertices[i];
    //
    //         target = l.g.nodeFromId(target_id);
    //         Arc a = findArc(l.g, source, target);
    //
    //         int arc_peso =  l.weight[a];
    //         int node_peso = l.weight_node[target];
    //
    //         if(((s.cost + arc_peso) * node_peso) < lowest_cost){
    //             lowest_cost = (s.cost + arc_peso) * node_peso;
    //             lowest_id = target_id;
    //             id_to_remove = i;
    //         }
    //     }
    //
    //     target = l.g.nodeFromId(lowest_id);
    //     Arc min_arc = findArc(l.g, source, target);
    //     int min_arc_peso =  l.weight[min_arc];
    //     int min_node_peso = l.weight_node[target];
    //     sum_ta += min_arc_peso;
    //     s.cost += min_node_peso * sum_ta;
    //
    //     // add vertice on the path
    //     path.push_back(lowest_id);
    //     s.tour.push_back(target);
    //
    //     #if DEBUG
    //     cout << "viz escolhido = " << avalibleVertices[id_to_remove] << " (k=" << id_to_remove << ")"<< endl;
    //     cout << "arc_peso = " << min_arc_peso << " node_peso = " << min_node_peso << endl;
    //     cout << "new sum = " << s.cost << endl << endl;
    //     #endif
    //
    //     // removes taken vertice from avalible vertices
    //     for(unsigned i = id_to_remove; i < avalibleVertices.size(); i++){
    //         avalibleVertices[i] = avalibleVertices[i+1];
    //     }
    //     avalibleVertices.pop_back();
    // }
    //
    // // add depot cost
    // source = l.g.nodeFromId(path.back());
    // Arc min_arc = findArc(l.g, source, l.depot);
    // sum_ta += l.weight[min_arc];
    // s.cost = sum_ta * l.weight_node[l.depot];
    //


    double arc_value, sum_tour = 0;
    int j, cost;
    for(int i = 1; i < l.n; i++){
        arc_value = DBL_MAX;

        for (OutArcIt a(l.g, s.tour.back()); a != INVALID; ++a){

            for(j = 0; j < i && l.g.target(a) != s.tour[j]; j++);
            if(j < i) continue;

            cost = (sum_tour + l.weight[a]) * l.weight_node[l.g.target(a)];

            if(cost <= arc_value){
                arc_value = cost;
                arc_min = a;
            }
        }

        sum_tour += l.weight[arc_min];
        s.cost += l.weight_node[l.g.target(arc_min)] * sum_tour;
        s.tour.push_back(l.g.target(arc_min));
    }
    #if DEBUG
    cout << "sum = " << s.cost << endl;
    #endif
    return false;
}

//------------------------------------------------------------------------------
#define INITAL_TEMPERATURE 100
#define MIN_TEMPERATURE 10
#define INNER_LIMIT 100 // inner loop limit
#define ALPHA_PARAM 0.85 // alpha parameter of geometric decay
#define K_PARAM 2 // k parameter that multiplies temperature

bool metaHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    /*  simulated annealing implementation  */

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

    #if DEBUG
    cout << "depot " << l.g.id(l.depot) << endl;

    for(int i = 0; i < tour_size; i++){
        cout << l.g.id(l.g.nodeFromId(i)) << endl;
    }
    #endif

    // a vector with the index of the current path
    vector<int> current_path(tour_size);
    current_path[0] = depot_id;

    // initial path (sorted indicies)
    for(int i = 1, j = 0; i < tour_size; i++){
        if(j == depot_id)
            j++;
        current_path[i] = j++;
    }

    int current_value = pathCost(l, current_path);

    // variables to store global best values
    int best_value = current_value;
    vector<int> best_path = current_path;

    #if DEBUG
    printVector(current_path);
    cout << "solucao inicial: ";
    cout << " valor: " << current_value << endl;
    #endif

    // while temperature hasnt achieved the minimal temperature
    while(temperature > MIN_TEMPERATURE){
        #if DEBUG
        cout << "temperature: " << temperature << endl;
        #endif

        // while it hasnt achieved the inner_limit or hasnt a new best global
        for(int i = 0; i < INNER_LIMIT; i++){
            // time's up, return current best solution
            if (timeout) {
                temperature = MIN_TEMPERATURE;
                break;
            }

            // neighbor solution = 2-OPT
            vector<int> neighbor_path = opt2(current_path, l);
            int neighbor_value = pathCost(l, neighbor_path);

            double diff = neighbor_value - current_value;

            #if DEBUG
            cout << "solucao vizinha: ";
            printVector(neighbor_path);
            cout << " valor: " << neighbor_value << endl;
            cout << "dif: " << diff << endl;
            #endif

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

                #if DEBUG
                cout << "rand: " << random_value << " e: " << e << endl;
                #endif

                // gets the new value
                if (e > random_value){
                    current_path = neighbor_path;
                    current_value = neighbor_value;

                    #if DEBUG
                    cout << "trocou" << endl;
                    #endif
                }

            }
        }
        // temperature's geometric decay
        temperature = temperature * ALPHA_PARAM;
    }

    cout << "solucao final: ";
    printVector(best_path);
    cout << " valor: " << best_value << endl;

    // set's current best global solution as solution
    s.cost = best_value;

    s.tour.clear();
    s.tour.push_back(l.depot);

    for(int i = 1; i < tour_size; i++){
        lemon::ListDigraphBase::Node node = l.g.nodeFromId(best_path[i]);
        s.tour.push_back(node);
    }

    return false;
}
//------------------------------------------------------------------------------
bool brkga(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    const clock_t begin = clock();

    tsp_p_Decoder decoder(l);

    const long unsigned rngSeed = time(0);	// seed to the random number generator
	MTRand rng(rngSeed);					// initialize the random number generator

	const unsigned n = l.n-1;		// size of chromosomes
	const unsigned p = 256;		// size of population
	const double pe = 0.10;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 3;		// number of independent populations
    const unsigned MAXT = 2; // number of threads for parallel decoding

    BRKGA< tsp_p_Decoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

    // BRKGA inner loop (evolution) configuration: Exchange top individuals
	const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 1000 gens

	// BRKGA evolution configuration: restart strategy
	unsigned relevantGeneration = 1;	// last relevant generation: best updated or reset called
	const unsigned RESET_AFTER = 200;
	std::vector< double > bestChromosome;
    double bestFitness = std::numeric_limits< double >::max();

    std::cout << "Running for " << MAX_GENS << " generations without multi-threading..." << std::endl;

    // Run the evolution loop:
	unsigned generation = 1;		// current generation
	do {
		algorithm.evolve();	// evolve the population for one generation

		// Bookeeping: has the best solution thus far improved?
		if(algorithm.getBestFitness() < bestFitness) {
			// Save the best solution to be used after the evolution chain:
			relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();

			std::cout << "\t" << generation
					<< ") Improved best solution thus far: "
					<< bestFitness << std::endl;
		}

		//  Evolution strategy: restart
		if(generation - relevantGeneration > RESET_AFTER) {
			algorithm.reset();	// restart the algorithm with random keys
			relevantGeneration = generation;

			std::cout << "\t" << generation << ") Reset at generation "
					<< generation << std::endl;
		}

		// Evolution strategy: exchange top individuals among the populations
		if(generation % X_INTVL == 0 && relevantGeneration != generation) {
			algorithm.exchangeElite(X_NUMBER);

			std::cout << "\t" << generation
					<< ") Exchanged top individuals." << std::endl;
		}

		// Next generation?
		++generation;
	} while (generation < MAX_GENS);

	// print the fitness of the top 10 individuals of each population:
	std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	for(unsigned i = 0; i < K; ++i) {
		std::cout << "Population #" << i << ":" << std::endl;
		for(unsigned j = 0; j < bound; ++j) {
			std::cout << "\t" << j << ") "
					<< algorithm.getPopulation(i).getFitness(j) << std::endl;
		}
	}

	// rebuild the best solution:
    DNode depot;
    s.tour.clear();
    s.cost = bestFitness;

    depot = l.depot; //tour begins at s
    s.tour.push_back(depot);

    vector<pair<int, double>> tour(bestChromosome.size());

    for(unsigned i = 0, j = 0; i < bestChromosome.size(); i++, j++) {
		if((int) i == l.g.id(depot))
			j++;
		tour[i] = make_pair(j, bestChromosome[i]);
	}

	std::sort(tour.begin(), tour.end(), comparator);

	vector<int> path(bestChromosome.size()+1);

	for(unsigned i = 0; i < bestChromosome.size(); i++)
		path[i+1] = tour[i].first;

    path[0] = l.g.id(l.depot);

    for(unsigned i = 0; i < path.size(); i++)
        cout << path[i] << " ";
    cout << endl;


    for(unsigned i = 1; i < bestChromosome.size()+1; i++){
        lemon::ListDigraphBase::Node w = l.g.nodeFromId(path[i]);
        cout << l.g.id(w) << " ";
        s.tour.push_back(w);
    }

    cout << endl;

	// print its distance:
	std::cout << "Best solution found has objective value = "
	 		<< pathCost(l, path) << std::endl;

	// print its best tour:
	std::cout << "Best tour:";
    printVector(path);
	std::cout << std::endl;

	const clock_t end = clock();
    std::cout << "BRKGA run finished in " << (end - begin) / double(CLOCKS_PER_SEC) << " s." << std::endl;

    return false;
}
//------------------------------------------------------------------------------
bool exact(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
   return naive(l, s, tl);
}
//------------------------------------------------------------------------------
bool naive(const Tsp_P_Instance &instance, Tsp_P_Solution  &sol, int tl)
/*
    *This simple algorithm just computes a tour by choosing the outArc with lowest t_a.
    *This tour, of course, begins at node s (depot);
*/
{
   DNode v, vl;
   sol.tour.clear();
   sol.cost = 0.0;

   v = instance.depot;//tour begins at s
   sol.tour.push_back(v);

   double vl_arc, sum_ta;
   OutArcIt arc_min;
   int i;


   sum_ta=0.0;

   while((int)sol.tour.size() < instance.n){
        vl_arc = DBL_MAX;

        for (OutArcIt a(instance.g, sol.tour.back()); a != INVALID; ++a){

            i = 0;
            while(i < (int)sol.tour.size() && instance.g.target(a) != sol.tour[i]) i++;
            if(i < (int)sol.tour.size()) continue;

            if(instance.weight[a] <= vl_arc){

                vl_arc = instance.weight[a];
                arc_min = a;
            }
        }
        sum_ta += instance.weight[arc_min];
        sol.cost += instance.weight_node[instance.g.target(arc_min)] * sum_ta;
        vl = instance.g.target(arc_min);
        sol.tour.push_back(vl);

    }
    return false;
}
//------------------------------------------------------------------------------

bool comparator (pair<int, double> i, pair<int, double> j) {
	return (i.second < j.second);
}

double pathCost(const Tsp_P_Instance &l, vector<int> path){

    double calcCost = 0.0;
    double sum_ta=0.0;

    lemon::ListDigraphBase::Node n, m;


    int tour_size = path.size();
    for(int i = 0; i < tour_size-1; i++){

        n = l.g.nodeFromId(path[i]);
        m = l.g.nodeFromId(path[i+1]);

        Arc a = findArc(l.g, n, m);

        // cout << "arc peso: " << l.weight[a];
        // cout << " node peso: " << l.weight_node[m] << endl;

        sum_ta += l.weight[a];
        calcCost += l.weight_node[m] * sum_ta;
    }

    //cout << "custo: " << calcCost << endl;

    return calcCost;
}

vector<int> getSolucaoVizinha(vector<int> v, int tour_size){
    vector<int> r = v;
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

vector<int> opt2Swap(vector<int> path, int j,  int k){
    vector<int> r = path;

    if ((j > k) || k >= (int) path.size())
        return path;

    for(int i = 0; i < ceil((k-j)/2.0); i++){
        int aux = r[j+i];
        r[j+i] = r[k-i];
        r[k-i] = aux;
    }

    return r;

}

vector<int> opt2(vector<int> path, const Tsp_P_Instance &l){
    vector<int> r = path;

    int best_value = pathCost(l, path);
    int tour_size = path.size();
    bool improved = true;

    while(improved){
        improved = false;
        for(int i = 1; i < tour_size - 1; i++){
            for(int j = i + 1; j < tour_size; j++){
                vector<int>aux = opt2Swap(r, i, j);
                int local_cost = pathCost(l, aux);
                if (local_cost < best_value){
                    best_value = local_cost;
                    r = aux;
                    improved = true;
                    break;
                }
            }
        }
    }

    return r;
}

vector<int> getFirstSolution(int depot_id, int tour_size){
    vector<int> v(tour_size);

    v[0] = depot_id;

    int k = 0;
    for(int i = 1; i < tour_size; i++){
        if(k == depot_id)
            k++;
        v[i] = k++;
    }

    return v;
}
