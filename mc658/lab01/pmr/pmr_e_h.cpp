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
int getRelations(vector<int> itens, matriz &relation, int i){

    int sum = 0;

    for (int j : itens){
        if(j != 0 && j != i)
            sum += relation[i][j];
    }

	return sum;
}

// sum the value, weight and relations of an iten's subset
int sumItens(int capacity, vector<int> s, vector<int> v, matriz &relation,
			 vector<int> itens, int quantItens){
	int value = 0;
	int weight = 0;

	for(int i = 0; i < quantItens; i++){
		weight += s[i];
		value += s[i] + getRelations(itens, relation, i);
		if (weight > capacity) return -1;
	}

	return value;
}

// generate all the combinations given a pivot
// matriz generateSubSet(int pivot, int quantItens){
//     matriz subset;
//
//     for(int i = pivot+1; i < quantItens; i++){
//         matriz result = generateSubSet(i, quantItens);
//         subset.insert();
//     }
//
//     return subset;
// }

int algE(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	// initializing an empty solution
	solution *sol;
	sol = new solution;

	sol->value = 0;
	sol->itens.reserve(quantItens);

	// interate over items
	for(int i = 0; i < quantItens; i++){
		int sol_size = 1;

		while(sol_size < quantItens){
			for(int j = i + sol_size; j < quantItens; j++){



			}
		}

	}


	return sol->value;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	return 0;
}
