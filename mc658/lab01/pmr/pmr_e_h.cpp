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
int getRelations(vector<int> itens, matriz &relation, int i, int quantItens){

    int sum = 0;

    for (int j = i; j < itens[quantItens]; j++)
        sum += relation[itens[i]][itens[j]];

	return sum;
}

// sum the value, weight and relations of an iten's subset
int sumItens(int capacity, vector<int> s, vector<int> v, matriz &relation,
			 vector<int> itens, int quantItens){
	int value = 0;

	for(int i = 0; i < itens[quantItens]; i++){
		value += v[itens[i]] + getRelations(itens, relation, i, quantItens);
	}

	return value;
}

bool sumWeights(vector<int> s, vector<int> itens, int capacity, int quantItens){
    int weight = 0;

	for(int i = 0; i < itens[quantItens]; i++){
        weight += s[itens[i]];
    }

    if (weight > capacity){
      return false;
    }
    return true;
}

// generate all the combinations
vector<vector<int> > generateCombinations(int quantItens,
  int capacity, vector<int> s, vector<int> v, solution *sol, matriz &relation){

  queue < vector<int> > fila;
  vector<vector<int> > combinations;

  for(int i = 0; i < quantItens; i++){
    vector<int> v(quantItens+1);
    //v.reserve(quantItens+1);
    v[0] = -1;
    v[quantItens] = 0;
    fila.push(v);
  }

  while (!fila.empty()) {
    vector<int> k = fila.front();


    //combinations.push_back(k);

    int soma;
    for(int i = k[k[quantItens]]+1; i < quantItens; i++){
      //k.push_back(i);
      k[k[quantItens]] = i;
      k[quantItens] += 1;


      soma = 0;
      if(sumWeights(s, k, capacity, quantItens)){
        soma = sumItens(capacity, s, v, relation, k, quantItens);

        std::cout << "itens: " << k[quantItens] << '\n';

        for(int j = 0; j < k[quantItens]; j++)
          cout << k[j] << ' ';
        cout << "\nsoma: " << soma << '\n';

        if (soma > sol->value){
          sol->value = soma;
          sol->itens = k;
        }
        fila.push(k);
      }

      k[quantItens] -= 1;
    }

    fila.pop();
  }

  return combinations;
}

// void prepareSolution(solution sol, vector<int> itens){
//   for(int i = 0; i < itens.size(); i++)
//     sol->itens[itens[i]] = 1;
// }

int algE(int capacity, int quantItens, vector<int> s, vector<int> v,
  matriz &relation, vector<int>& itensMochila, int maxTime)
{
	// initializing an empty solution
	solution *sol;
	sol = new solution;

	sol->value = 0;
	sol->itens.reserve(quantItens);

    vector<vector<int> > combinations = generateCombinations(quantItens,
                                          capacity, s, v, sol, relation);

    vector<int> aux;
    // for(int j = 0; j < combinations.size(); j++){
    //
    //     aux = combinations[j];
    //     soma = sumItens(capacity, s, v, relation, aux, quantItens);
    //
    //     if (soma > sol->value){
    //       sol->value = soma;
    //       sol->itens = aux;
    //     }
    // }

    for(int i = 0; i < sol->itens[quantItens]; i++)
        itensMochila[sol->itens[i]] = 1;

    return sol->value;
}

int algH(int capacity, int quantItens, vector<int> s, vector<int> v, matriz &relation, vector<int>& itensMochila, int maxTime)
{
	return 0;
}
