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
#include "gurobi_c++.h"

#define DEBUG 0
#define OUTARCIT 0

void findsubtour(int n, double** sol, int* tourlenP, int* tour);


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

bool naive(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);

double pathCost(const Tsp_P_Instance &l, vector<int> path);

vector<int> getRandomNeighbor(vector<int> v, int tour_size);

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
    #if !OUTARCIT
    // auxiliar variables
    int lowest_cost = (int) DBL_MAX;
    int lowest_id = 0;
    int id_to_remove = 0;
    int sum_ta = 0;

    // gets the insertion with the lowest impact on cost
    while(!avalibleVertices.empty()){
        if (timeout) break;
        lowest_cost = (int) DBL_MAX;

        #if DEBUG
        printVector(path);
        printVector(avalibleVertices);
        #endif

        // get the last vertice on the path
        int source_id = path.back();
        source = l.g.nodeFromId(source_id);

        #if DEBUG
        cout << "id = " << source_id << endl;
        #endif

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

        #if DEBUG
        cout << "viz escolhido = " << avalibleVertices[id_to_remove] << " (k=" << id_to_remove << ")"<< endl;
        cout << "arc_peso = " << min_arc_peso << " node_peso = " << min_node_peso << endl;
        cout << "new sum = " << s.cost << endl << endl;
        #endif

        // removes taken vertice from avalible vertices
        for(unsigned i = id_to_remove; i < avalibleVertices.size(); i++){
            avalibleVertices[i] = avalibleVertices[i+1];
        }
        avalibleVertices.pop_back();
    }
    #endif


    #if OUTARCIT
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
    #endif

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
    /*  simulated annealing implementation for TSP-P  */

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
        DNode node = l.g.nodeFromId(best_path[i]);
        s.tour.push_back(node);
    }

    return false;
}

//------------------------------------------------------------------------------
/* BRKGA constants */
#define P 256 // size of population
#define PE 0.1 // fraction of population to be the elite-set
#define PM 0.10 // fraction of population to be replaced by mutants
#define RHOE 0.70 // probability that offspring inherit an allele from elite parent
#define K 3 // number of independent populations
#define MAXT 2 // number of threads for parallel decoding

/* BRKGA inner loop (evolution) configuration: Exchange top individuals */
#define X_INTVL 100 // exchange best individuals at every 100 generations
#define X_NUMBER 2 // exchange top 2 best
#define MAX_GENS 1000 // run for 1000 generations

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

    BRKGA< tsp_p_Decoder, MTRand > algorithm(
        tour_size, P, PE, PM, RHOE, decoder, rng, K, MAXT);

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

  // http://www.gurobi.com/documentation/7.5/examples/tsp_cpp_cpp.html

  try {
    int i, j, n = l.n;
    GRBEnv *env = NULL;
    env = new GRBEnv();
    GRBModel model = GRBModel(*env);

    model.set(GRB_IntParam_LazyConstraints, 1);

    // x_ij = 0 || 1
    GRBVar **x = NULL;

    x = new GRBVar*[n];
    for (i = 0; i < n; i++)
        x[i] = new GRBVar[n];

    GRBVar *tempo = new GRBVar[n];

    // calculates the sum of all times
    // find most pesado arc
    double total_time = 0;
    double biggest_time = 0;
    for(ListDigraph::ArcIt arc(l.g); arc != INVALID; ++arc){
        double current_time = l.weight[arc];
        if(current_time > biggest_time)
            biggest_time = current_time;
    }

    total_time = biggest_time * n;

	cout << "pesadaum " << total_time << endl;

    // vetor de nos
    //vector<int> nodes_id;
    vector<DNode> nodes(n);
    for(ListDigraph::NodeIt node(l.g); node != INVALID; ++node){
        //nodes_id.push_back(l.g.id(node));
        nodes[l.g.id(node)] = node;
    }
    

    // tempo de cada no
    i = 0;
    for(int i = 0; i < n; i++){
        cout << "id: " << i << " peso: " << l.weight_node[nodes[i]] << endl;
        tempo[i] = model.addVar(0.0, total_time, l.weight_node[nodes[i]], GRB_CONTINUOUS, "t");
}

    // x_ij = variavel que determina se arco esta ou nao na solucao
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            x[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "x");
        }
    }

    // sum x_ij = 2
    // Degree-2 constraints
    for (i = 0; i < n; i++) {
      GRBLinExpr expr_in = 0;
      GRBLinExpr expr_out = 0;
      for (j = 0; j < n; j++){
        expr_in += x[i][j];
        expr_out += x[j][i];
      }
      model.addConstr(expr_out == 1);
      model.addConstr(expr_in == 1);
    }

    model.addConstr(tempo[l.g.id(l.depot)] == 0);
    cout << "depot id: " << l.g.id(l.depot) << endl;
    // v = tempo[]
    // vj >= vi + tij - (1-xij)M


    for (i = 0; i < n; i++){
        if (i == l.g.id(l.depot)) continue;

        for (j = 0; j < n; j++){
            if (i == j)
                x[i][i].set(GRB_DoubleAttr_UB, 0);

            else {
                double value = l.weight[findArc(l.g, nodes[i], nodes[j])];
                model.addConstr(tempo[i] >= tempo[j] + value - (1 - x[i][j]) * total_time);
            }
        }
    }
     // cout << "arco(" << i << "," << j << "): " << value << endl;
    // vj <= vi + tij + (1-xij)M

    // sum x_ii = 0
    //for (i = 0; i < n; i++)
    //    x[i][i].set(GRB_DoubleAttr_UB, 0);

    subtourelim cb = subtourelim(x, n);
    model.setCallback(&cb);

    model.update();
    model.optimize();

    if (model.get(GRB_IntAttr_SolCount) > 0) {
      double **sol = new double*[n];
      for (i = 0; i < n; i++)
        sol[i] = model.get(GRB_DoubleAttr_X, x[i], n);

      int* tour = new int[n];
      int len;

      findsubtour(n, sol, &len, tour);
      if(len < n) cout << "eita" << endl;

      cout << "Tour: ";
      for (i = 0; i < len; i++){
        cout << tour[i] << " ";
        s.tour.push_back(nodes[tour[i]]);
      }
      cout << endl;

      for (i = 0; i < n; i++)
        delete[] sol[i];
      delete[] sol;
      delete[] tour;
      
      s.cost = model.getObjective().getValue();

      return false;
     
    }

    } catch (GRBException e) {
      cout << "Error number: " << e.getErrorCode() << endl;
      cout << e.getMessage() << endl;
    } catch (...) {
      cout << "Error during optimization" << endl;
    }

       
    //return naive(l, s, tl);
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

static void alarm_handler(int sig)
{
    timeout = 1;
}

bool comparator (pair<int, double> i, pair<int, double> j) {
    return (i.second < j.second);
}

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

vector<int> getRandomNeighbor(vector<int> v, int tour_size){
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

void findsubtour(int      n, double** sol, int*     tourlenP, int*     tour)
{
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
