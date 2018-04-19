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
#include "pmr_e_h.h"
//typedef vector<vector<double> > matriz;

// solution struct
typedef struct _solution{
    int value;
    vector<int> itens;
} solution;

// sum the relations of the new item with the objects that already
// are on the solution
int getRelations(vector<int> &itens, matriz &relation, int last, int quantItens){

    int sum = 0;
    int size = itens.size();
    for (int j = 0; j < size; j++)
        sum += relation[itens[j]][last];

	return sum;
}

// this function gets a combination, calculate it's sum and if it's a valid
// combination, call the function for it sons
void getSum(int quantItens, matriz &relation, vector<int> &combination, int actual_sum, solution *sol, vector<int> &v, vector<int> &s, int capacity){
    // for (int j =0, k=0; j < quantItens; j++)
    //     if (k <combination.size() && combination[k] == j) {
    //         cout << 1;
    //         k++;
    //     }
    //     else
    //         cout << 0;
    // cout << endl << actual_sum << endl;


    for(int i = combination.back()+1; i < quantItens; i++){
        if (s[i] <= capacity) {
            combination.push_back(i);
            int sum = actual_sum + v[i];
            sum += getRelations(combination, relation, i, quantItens);
            if (sum > sol->value){
                sol->value = sum;
                sol->itens = combination;
            }
            if(s[i] < capacity){
                getSum(quantItens, relation, combination, sum, sol, v, s,    capacity-s[i]);
            }
            combination.pop_back();
        }
    }
}

// generate all the combinations
void getSolution(int quantItens, int capacity, vector<int> &s, vector<int> &v, matriz &relation, solution *sol){

  for(int i = 0; i < quantItens; i++){
      vector<int> init;
      init.push_back(i);
      if(s[i] <= capacity){
          int sum = v[i];
          if (sum > sol->value){
              sol->value = sum;
              sol->itens = init;
          }
          if(s[i] < capacity){
              getSum(quantItens, relation, init, v[i], sol, v, s, capacity-s[i]);
        }
      }
  }

}


int algE(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	// initializing an empty solution
	solution *sol;
	sol = new solution;
	sol->value = 0;
	sol->itens.reserve(quantItens);

    getSolution(quantItens, capacity, s, v, relation, sol);

    // translate vector of index -> binary vector
    int size = sol->itens.size();
    for(int i = 0; i < size; i++){
        itensMochila[sol->itens[i]] = 1;
        cout << sol->itens[i] + 1 << " ";
    }
    cout << endl;

    //cout << sumWeights(s, sol->itens) << endl;

    return sol->value;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	return 0;
}
