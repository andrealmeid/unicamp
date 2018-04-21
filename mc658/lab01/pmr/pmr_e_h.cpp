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

// struct to hold the original index of the item and the relative value
// relative value = (sum of all the relations + individual value)/weight
typedef struct _item {
	int index;
	double relative_value;
} item;

// item struct comparator
bool compareItems(item* lhs, item* rhs);

// calculates a relative value of each item and sorted they by this value
vector<int> sortedItens(matriz &relation, vector<int> v, vector<int> s, int quantItens);


/* sum the relations of the new item with the objects that already
   are on the solution */
int getRelations(vector<int> &itens, matriz &relation, int row, int quantItens);


/* this function gets a combination, calculate it's sum and if it's a valid
   combination, call the function for it sons */
void getSolutionSons(int quantItens, matriz &relation, vector<int> &combination,
            int actual_sum, vector<int> &v, vector<int> &s,
            int capacity);


/* generate the solutions parents (one single element on the bag) and calls
   a recursive function to add more itens to the solution */
void getSolution(int quantItens, int capacity, vector<int> &s, vector<int> &v,
			                 matriz &relation);


// alarm functions
volatile sig_atomic_t timeout = 0;
static void alarm_handler(int sig);


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


int algH(int capacity, int quantItens, vector<int> s, vector<int> v,
         matriz &relation, vector<int>& itensMochila, int maxTime)
{
	signal(SIGALRM, alarm_handler);
	alarm(maxTime);
    vector<int> items = sortedItens(relation, v, s, quantItens);

    int sum = 0;

    vector<int> solution;

    // we gonna put how many items we can, according to the priority order
	for (int i = 0; i < quantItens; i++) {
		if (timeout) break;
		int actual_item = items[i];

		// if actual item doesnt fit, go to next item
		if(s[actual_item] <= capacity){
			solution.push_back(actual_item);
			sum += v[actual_item];
			sum += getRelations(solution, relation, actual_item, quantItens);
			capacity -= s[actual_item];
		}
	}

	// translate vector of indeces to a binary vector
	int size = solution.size();
	for(int i = 0; i < size; i++)
        itensMochila[solution[i]] = 1;

	return sum;
}

static void alarm_handler(int sig)
{
    timeout = 1;
}

bool compareItems(item* lhs, item* rhs)
{
  return lhs->relative_value > rhs->relative_value;
}


vector<int> sortedItens(matriz &relation, vector<int> v, vector<int> s, int quantItens){

    vector<int> order(quantItens);
    vector<item *> items(quantItens);

    item *o;
    double sum;

	/* for each item, sums all the relation values, the value and divide by
	   the weight */
    for(int i = 0; i < quantItens; i++){
        o = new item;
        o->index = i;
        sum = v[i];

        for(int j = 0; j < quantItens; j++)
            sum += relation[i][j];

        o->relative_value = sum/s[i];
        items[i] = o;
    }

	// sort items
    sort(items.begin(), items.end(), compareItems);

	// cast vector<item> to vector<int>
    for(int i = 0; i < quantItens; i++)
        order[i] = items[i]->index;

    return order;
}

int getRelations(vector<int> &itens, matriz &relation, int row, int quantItens){

    int sum = 0;
    int size = itens.size();

    for (int i = 0; i < size; i++)
        sum += relation[itens[i]][row];

	return sum;
}

void getSolutionSons(int quantItens, matriz &relation, vector<int> &combination,
            int actual_sum, vector<int> &v, vector<int> &s,
            int capacity){

    /* beging to the next item to the last item, inserts
       a new element on the solution, calculates the solution and if it
      remains a space, call the function to it sons */

    for(int i = combination.back()+1; i < quantItens; i++){
		// stop making computation when times up
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
                getSolutionSons(quantItens, relation, combination, sum, v, s,
                       capacity-item_s);

            combination.pop_back();
        }
    }
}

// generate the solutions parents (one single element on the bag) and calls
// a recursive function to add more itens to the solution
void getSolution(int quantItens, int capacity, vector<int> &s, vector<int> &v,
                 matriz &relation){

	vector<int> itens = sortedItens(relation, v, s, quantItens);

    // for each single element...
    for(int i = 0; i < quantItens; i++){
      vector<int> init;
	  int actual_item = itens[i];
      init.push_back(actual_item);

      // if the element is of the size of the bag, place it on the bag
      // and check if it's a good solution
      if(s[actual_item] <= capacity){

          int value = v[actual_item];
          if (value > sol->value){
              sol->value = value;
              sol->itens = init;
          }

          // if the element fits on the bag and left an empty space, call the
          // function to place more itens
          if(s[actual_item] < capacity)
              getSolutionSons(quantItens, relation, init, v[actual_item], v, s, capacity-s[actual_item]);
      }

    }

}
