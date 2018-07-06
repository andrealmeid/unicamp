/* RA 164047
   André Figueiredo de Almeida
   Lab 03 - Escola */

/*  Corretude: o algoritmo funciona pegando cada aluno e aplicando o algoritmo de
  djikstra para cada aluno em busca da escola mais próxima. O aluno vai encontrar
  a escola mais proxima dele porque o algoritmo de djikstra (implementado em
  getSchool) retorna o caminho menos custoso em um grafo de arestas positivas.
    Complexidade: o código irá aplicar djikstra em cada aluno, resultando em
  uma complexidade de O(VE + V²log(V)) do programa. Existe também uma ordenação
  dos alunos, com complexidade O(V²logV).
*/

#include "solve.hpp"

/*
  estudante: vértice que está procurando escola
  g: vetor de listas de adjacencia
  e: escolas/pontos de onibus
  vertices: guarda os vertices do grafo
*/
int getSchool(int estudante, vector< vector< pair<int, int> > > &g, vector< vertice> &vertices){

  //verifica se aluno mora na escola
  if(vertices[estudante].escola != INF)
    return vertices[estudante].escola;

  // min heap para os vertices dos caminhos
  priority_queue <vertice, vector<vertice>, comp>  heap;

  // pega os vizinhos do estudante e poe no heap
  vertice v;
  for (auto vizinho : g[estudante]){
    v = vertices[vizinho.first];
    v.dist = vizinho.second;
    heap.push(v);
  }

  // enquanto tiver algum vertice no heap
  while(!heap.empty()){
    // pega o primeiro
    v = heap.top();
    heap.pop();

    // se ele ja foi visitado, ignora
    if(v.visitado)
      continue;
    v.visitado = true;

    // se ele tem uma escola, retorna a escola
    if (v.escola != NOT_SCHOOL){
      return v.escola;
    }

    // pega os vizinhos do vertice atual e poe no heap
    vertice aux;
    for(auto viz : g[v.id]){
      aux = vertices[viz.first];
      aux.dist = viz.second + v.dist;
      heap.push(aux);
    }
  }

  return 0;
}

vector< vector<int> > solve(vector< vector< pair<int, int> > > &g, vector< vector <int> > &e, vector <int> &s){

  vector< vector<int> > solution(e.size()); // estrutura de solução, cada linha é uma escola, cada celula é uma casa

  vector< vertice> vertices;

  // Cria um vértice de referência para inicializar todos
  vertice v;
  v.dist = INF;
  v.escola = NOT_SCHOOL;
  v.visitado = false;

  // Inicializa todos vértices
  for (unsigned int i = 0; i < g.size(); i++){
    v.id = i;
    vertices.push_back(v);
  }

  // Pega o valor da escola e o replica para seus pontos de ônibus no vetor de vértices
  for (unsigned int i = 0; i < e.size(); i++)
    for (unsigned int j = 0; j < e[i].size(); j++)
        if (vertices[e[i][j]].escola == NOT_SCHOOL)
          vertices[e[i][j]].escola = i;


  // Para cada aluno calcula a escola mais próxima
  for (auto student : s){
    int school = getSchool(student, g, vertices);
    solution[school].push_back(student);
  }

  // Ordena os alunos de cada solução
  for (unsigned int i = 0; i < solution.size(); i++){
    sort(solution[i].begin(), solution[i].end());
  }

  return solution;
}
