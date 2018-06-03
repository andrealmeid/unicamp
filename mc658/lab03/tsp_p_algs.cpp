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
#include "mygraphlib.h"
#include "tsp_p_algs.h"
#include <algorithm>
#include <stdlib.h>
#include <time.h>

bool naive(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);

double pathCost(const Tsp_P_Instance &l, vector<int> path);

vector<int> getSolucaoVizinha(vector<int> v, int tour_size);

void printVector(vector<int> v){
    for(int i = 0; i < (int)v.size(); i++)
        cout << v[i] << " ";
}

//------------------------------------------------------------------------------
bool constrHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    DNode depot;
    s.tour.clear();
    s.cost = 0.0;

    depot = l.depot; //tour begins at s
    s.tour.push_back(depot);

    double arc_value, sum_tour = 0;
    OutArcIt arc_min;
    int j, cost;

    for(int i = 1; i < l.n; i++){
        arc_value = DBL_MAX;

        for (OutArcIt a(l.g, s.tour.back()); a != INVALID; ++a){

            for(j = 0; j < i && l.g.target(a) != s.tour[j]; j++);
            if(j < i) continue;

            cost = l.weight[a] * l.weight_node[l.g.target(a)];

            if(cost <= arc_value){
                arc_value = cost;
                arc_min = a;
            }
        }

        sum_tour += l.weight[arc_min];
        s.cost += l.weight_node[l.g.target(arc_min)] * sum_tour;
        s.tour.push_back(l.g.target(arc_min));
    }

    return false;
}
//------------------------------------------------------------------------------
bool metaHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    // Tsp_P_Solution best;
    //

    // simulated annealing
    srand(time(NULL));
    int temperatura = 100;
    int n = 10;
    int tour_size = l.n;

    vector<int> v = {0, 1, 2, 3};
    printVector(v);
    int value = pathCost(l, v);

    int best_value = value;
    vector<int> best_path = v;

    cout << "solucao inicial: ";
    cout << " valor: " << value << endl;

    while(temperatura >= 10){
        cout << "temperatura: " << temperatura << endl;

        for(int i = 0; i < n; i++){
            value = pathCost(l, v);
            vector<int> v_viz = getSolucaoVizinha(v, tour_size-1);
            int value_viz = pathCost(l, v_viz);
            int dif = value_viz - value;

            cout << "solucao vizinha: ";
            printVector(v_viz);
            cout << " valor: " << value_viz << endl;

            cout << "dif: " << dif << endl;

            if (dif < 0){
                v = v_viz;

                if (value_viz < best_value){
                    best_value = value_viz;
                    best_path = v_viz;
                    i = n;
                }

            } else {
                float randomico = (rand() % 10) / 10.0;
                float param = ((float)(-dif)*1.0)/((float)temperatura * 1.0);
                float e = exp(param);
                cout << "rand: " << randomico << " e: " << e << endl;
                if (e > randomico){
                    v = v_viz;
                    cout << "trocou" << endl;
                }
            }
        }
        temperatura = temperatura * 0.80;
    }

    cout << "solucao final: ";
    printVector(best_path);
    cout << " valor: " << best_value << endl;

    DNode depot;
    s.tour.clear();
    s.cost = best_value;

    depot = l.depot; //tour begins at s
    s.tour.push_back(depot);

    for(int i = 1; i < best_path.size(); i++){
        lemon::ListDigraphBase::Node w = l.g.nodeFromId(best_path[i]);
        s.tour.push_back(w);
    }

    return false;
}
//------------------------------------------------------------------------------
bool brkga(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{

    return naive(l, s, tl);
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
