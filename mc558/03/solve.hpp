/*  RA: 164047 - MC558A
    André Figueiredo de Almeida
    Laboratorio 02 */


#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <cmath>
#include <cstdint>
#include <climits>
#include <fstream>
#include <queue>

using namespace std;

// Estrutura que guarda coordenadas do pixel e a distancia dele com sua raiz
typedef struct _pixel{
  int i;
  int j;
  int cost;
} pixel;

// Classe usada para comparacao entre dois pixels
class comp {
public:
    int operator() (const pixel& p1, const pixel& p2) {
        return p1.cost > p2.cost;
    }
};

int cost(vector< vector<int> > &image, pair<int, int> a, pair<int, int> b);
long long int mst_image(vector< vector<int> > &image, vector< pair<int, int> > &points);
pixel getCost(vector< vector<int> > &image, pair<int, int> ponto, pair<int, int> p);
void getNeighborhood(vector< vector<int> > &image, vector< vector<char> > &choosed, int i, int j,
              priority_queue<pixel, vector<pixel>, comp> &queue, int n, int m);
