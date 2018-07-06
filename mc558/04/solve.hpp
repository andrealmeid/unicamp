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
#include <utility>
#include <vector>
#include <algorithm>

#define INF INT_MAX
#define NOT_SCHOOL INT_MAX

using namespace std;

typedef struct _vertice {
  int id; // Número/indíce do vértice
  int dist; // Distância relativa a uma casa
  bool visitado; // Indica se o caminho já passou por ele
  int escola; // Escola a qual esse ponto leva
} vertice;


class comp {
public:
    int operator() (const vertice& v1, const vertice& v2) {
        if (v1.dist == v2.dist)
          return v1.escola > v2.escola;
        return v1.dist > v2.dist;
    }
};

int getSchool(int estudante, vector< vector< pair<int, int> > > &g, vector< vertice> &vertices);

vector< vector<int> > solve(vector< vector< pair<int, int> > > &g, vector< vector <int> > &e, vector <int> &s);
