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

/*
	s -> weight
	v -> value
*/

// sum the relations on a subset of itens
int getRelations(vector<int> itens, matriz &relation, int last, int quantItens){

    int sum = 0;

    for (int j = 0; j < itens.size(); j++)
        sum += relation[itens[j]][last];

	return sum;
}

// sum the value, weight and relations of an iten's subset
int sumItens(int capacity, vector<int> s, vector<int> v, matriz &relation,
			 vector<int> itens, int quantItens){
	int value = 0;

	for(int i = 0; i < itens.size(); i++){
		value += v[itens[i]] + getRelations(itens, relation, i, quantItens);
	}

	return value;
}

int sumWeights(vector<int> s, vector<int> itens){
    int weight = 0;

	for(int i = 0; i < itens.size(); i++){
        weight += s[itens[i]];
    }
    return weight;
}


void getSum(int quantItens, matriz &relation, vector<int> combination, int actual_sum, solution *sol, vector<int> v, vector<int> s, int capacity){
    // for (int j =0, k=0; j < quantItens; j++)
    //     if (k <combination.size() && combination[k] == j) {
    //         cout << 1;
    //         k++;
    //     }
    //     else
    //         cout << 0;
    // cout << endl << actual_sum << endl;

    int sum = actual_sum;
    sum += getRelations(combination, relation, combination.back(), quantItens);
    if (sum > sol->value){
        sol->value = sum;
        sol->itens = combination;
    }

    for(int i = combination.back()+1; i < quantItens; i++){
        if (s[i] <= capacity) {
            combination.push_back(i);
            getSum(quantItens, relation, combination, sum + v[i], sol, v, s, capacity-s[i]);
            combination.pop_back();
        }
    }
}

// generate all the combinations
void getSolution(int quantItens, int capacity, vector<int> s, vector<int> v, matriz &relation, solution *sol){

  for(int i = 0; i < quantItens; i++){
      vector<int> init;
      init.push_back(i);
      if(s[i] <= capacity)
        getSum(quantItens, relation, init, v[i], sol, v, s, capacity-s[i]);
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

    for(int i = 0; i < sol->itens.size(); i++){
        itensMochila[sol->itens[i]] = 1;
        cout << sol->itens[i] + 1 << " ";
    }
    cout << endl;

    cout << sumWeights(s, sol->itens) << endl;

    return sol->value;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	return 0;
}
