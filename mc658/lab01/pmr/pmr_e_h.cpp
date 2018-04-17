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
int getRelations(vector<int> itens, matriz &relation, int i){

    int sum = 0;

    for (int j = 0; j < itens.size(); j++){
        if(j != i)
            sum += relation[i][j];
    }

	return sum;
}

// sum the value, weight and relations of an iten's subset
int sumItens(int capacity, vector<int> s, vector<int> v, matriz &relation,
			 vector<int> itens, int quantItens){
	int value = 0;
	int weight = 0;

	for(int i = 0; i < itens.size(); i++){
		weight += s[i];
		value += s[i] + getRelations(itens, relation, i);
		//if (weight > capacity) return -1;
	}

	return value;
}

// generate all the combinations
std::vector<std::vector<int> > generateCombinations(int quantItens){
  queue < vector<int> > fila;
  std::vector<std::vector<int> > combinations;

  for(int i = 0; i < quantItens; i++){
    std::vector<int> v;
    v.push_back(i);
    fila.push(v);
  }

  while (!fila.empty()) {
    std::vector<int> k = fila.front();

    combinations.push_back(k);

    // for(int i = 0; i < k.size(); i++)
    //   std::cout << k[i] << ' ';
    // std::cout << '\n';

    for(int i = k.back()+1; i < quantItens; i++){
      k.push_back(i);
      fila.push(k);
      k.pop_back();
    }

    fila.pop();
  }

  return combinations;
}

// void prepareSolution(solution sol, vector<int> itens){
//   for(int i = 0; i < itens.size(); i++)
//     sol->itens[itens[i]] = 1;
// }

int algE(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	// initializing an empty solution
	solution *sol;
	sol = new solution;

	sol->value = 0;
	sol->itens.reserve(quantItens);

  std::vector<std::vector<int> > combinations = generateCombinations(quantItens);

  for(int j = 0; j < combinations.size(); j++){
    std::vector<int> v = combinations[j];

    // print combination
    for(int i = 0; i < v.size(); i++){
        std::cout << v[i] << ' ';
    }
    std::cout << '\n';

    int soma = sumItens(capacity, s, v, relation, v, quantItens);
    std::cout << "soma: " << soma << '\n';
    if (soma > sol->value){
      sol->value = soma;
      sol->itens = v;
    }
  }

  for(int i = 0; i < sol->itens.size(); i++)
    itensMochila[sol->itens[i]] = 1;

	return sol->value;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	return 0;
}
