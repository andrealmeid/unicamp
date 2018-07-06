/* 	Andre Figueiredo de Almeida
	RA: 164047
	MC558 Lab 02 */

#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <stack>

enum Color { WHITE, BLACK };

using namespace std;

typedef struct _user {
	int group;				// ID do grupo (ou componente) ao que pertence
	int color;				// cor: WHITE (nao visititado) ou BLACK (visitado)
	vector<int> neighbors;	// vetor de vizinhos do vertice
} user;

void search(vector<user> &users, user *root, int grupo);

void solve(int n, int m, vector< vector<int> > &userEmail, vector< vector<int> > &userSolution);
