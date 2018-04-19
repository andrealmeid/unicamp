/*******************************************************************************
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa
 ******************************************************************************/

/*******************************************************************************
 * EDITE ESTE ARQUIVO APENAS ONDE INDICADO
 * DIGITE SEU RA: 164047
 * SUBMETA SOMENTE ESTE ARQUIVO
 ******************************************************************************/

#include <iostream>
#include <float.h>
#include <queue>
#include <signal.h>
#include <unistd.h>
#include "pmr_e_h.h"
//typedef vector<vector<double> > matriz;

// struct that holds a solution
typedef struct _solution{
    int value;
    vector<int> itens;
} solution;

// global solution variable
solution *sol;

// alarm functions
volatile sig_atomic_t timeout = 0;
static void alarm_handler(int sig)
{
    timeout = 1;
}

// sum the relations of the new item with the objects that already
// are on the solution
int getRelations(vector<int> &itens, matriz &relation, int row, int quantItens){

    int sum = 0;
    int size = itens.size();

    for (int i = 0; i < size; i++)
        sum += relation[itens[i]][row];

	return sum;
}

// this function gets a combination, calculate it's sum and if it's a valid
// combination, call the function for it sons
void getSum(int quantItens, matriz &relation, vector<int> &combination,
            int actual_sum, vector<int> &v, vector<int> &s,
            int capacity){

    // beging to the next item to the last item, inserts
    // a new element on the solution, calculates the solution and if it
    // remains a space, call the function to it sons

    // stop making computation when times up
    for(int i = combination.back()+1; i < quantItens; i++){
        if (timeout) return;

        // gets the weight of the item
        int item_s = s[i];

        // checks if the object fit in the bag
        if (item_s <= capacity) {
            combination.push_back(i);
            int sum = actual_sum + v[i];
            sum += getRelations(combination, relation, i, quantItens);

            // if it's the best solution at time, updates sol
            if (sum > sol->value){
                sol->value = sum;
                sol->itens = combination;
            }

            // if the element fits on the bag and left an empty space, call the
            // function to place more itens
            if(item_s < capacity)
                getSum(quantItens, relation, combination, sum, v, s,
                       capacity-item_s);

            combination.pop_back();
        }
    }
}

// generate the solutions parents (one single element on the bag) and calls
// a recursive function to add more itens to the solution
void getSolution(int quantItens, int capacity, vector<int> &s, vector<int> &v,
                 matriz &relation){

    // for each single element...
    for(int i = 0; i < quantItens; i++){
      vector<int> init;
      init.push_back(i);

      // if the element is of the size of the bag, place it on the bag
      // and check if it's a good solution
      if(s[i] <= capacity){

          int value = v[i];
          if (value > sol->value){
              sol->value = value;
              sol->itens = init;
          }

          // if the element fits on the bag and left an empty space, call the
          // function to place more itens
          if(s[i] < capacity)
              getSum(quantItens, relation, init, v[i], v, s, capacity-s[i]);
      }

    }

}

int algE(int capacity, int quantItens, vector<int> s, vector<int> v,
         matriz &relation, vector<int>& itensMochila, int maxTime)
{
    // initialize alarm
    signal(SIGALRM, alarm_handler);
    alarm(maxTime);


	// initializing an empty solution
	sol = new solution;
	sol->value = 0;
    sol->itens.reserve(quantItens);

    // calculate the exact solution
    getSolution(quantItens, capacity, s, v, relation);

    // translate vector of indeces to a binary vector
    int size = sol->itens.size();
    for(int i = 0; i < size; i++)
        itensMochila[sol->itens[i]] = 1;

    return sol->value;
}

typedef struct _item {
    int index;
    double relative_value;
} item;

vector<int> orderedItens(matriz &relation, vector<int> v, vector<int> s, int quantItens){
    vector<int> order(quantItens);

    vector<item *> items(quantItens);

    item *o;
    int sum;
    for(int i = 0; i < quantItens; i++){
        o = new item;
        o->index = i;
        sum = v[i];
        for(int j = 0; j < quantItens; j++)
            sum += relation[i][j];

        o->relative_value = sum/s[i];
        items[i] = o;
    }

    for(int i = 0; i < quantItens; i++){
        order[i] = items[i]->index;
    }

    return order;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v,
         matriz &relation, vector<int>& itensMochila, int maxTime)
{
    vector<int> items = orderedItens(relation, v, s, quantItens);
	return 0;
}
