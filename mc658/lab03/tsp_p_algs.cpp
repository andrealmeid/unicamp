/*******************************************************************************
 ** VERSION: 1.0
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa
 ******************************************************************************/

/* IMPLEMENTE AS FUNCOES INDICADAS
 * DIGITE SEU RA: ______
 * SUBMETA SOMENTE ESTE ARQUIVO */

#include <iostream>
#include <float.h>
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include "tsp_p_algs.h"
#include <algorithm>

bool naive(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);

//------------------------------------------------------------------------------
bool constrHeur(const Tsp_P_Instance &instance, Tsp_P_Solution  &sol, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
    DNode s, vl;
    sol.tour.clear();
    sol.cost = 0.0;

    s = instance.depot; //tour begins at s
    sol.tour.push_back(s);

    double vl_arc, sum_ta = 0;
    OutArcIt arc_min;
    int i, cost;

    for(int k = 1; k < instance.n; k++){
    //while((int)sol.tour.size() < instance.n){
        vl_arc = DBL_MAX;

        for (OutArcIt a(instance.g, sol.tour.back()); a != INVALID; ++a){

            i = 0;
            while(
                i < (int)sol.tour.size() && instance.g.target(a) != sol.tour[i]
            )
                i++;

            if(i < (int)sol.tour.size()) continue;

            cost = instance.weight[a] * instance.weight_node[instance.g.target(a)];

            if(cost <= vl_arc){
                vl_arc = cost;
                arc_min = a;
            }
        }

        cout << vl_arc  << " ";

        sum_ta += instance.weight[arc_min];
        sol.cost += instance.weight_node[instance.g.target(arc_min)] * sum_ta;
        vl = instance.g.target(arc_min);
        sol.tour.push_back(vl);

    }

    return false;
}
//------------------------------------------------------------------------------
bool metaHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl)
/* Implemente esta função, entretanto, não altere sua assinatura */
{
   return naive(l, s, tl);
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
