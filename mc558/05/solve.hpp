#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <stack>
#include <cmath>
#include <cstdint>
#include <climits>
#include <fstream>
#include <queue>
#include <utility>
#include <vector>

#define INF INT_MAX

using namespace std;

typedef struct {
    int repeticoes; // Numero de repetições permitidas
    vector<int> indexNaoSeq; //Indice dos brinquedos a não sequenciar
}atracao;

void updateFlow(vector<int> predecessors, vector< int > &capacity, vector<int> &flow, int last);

bool getMaxFlow(int inicio, int end, vector< int > &capacity, vector< int > &flow, vector< vector<int>> neighborhood, int v);

int solve(int v, vector<atracao> atracoes);
