/*  RA: 164047 - MC558A
    André Figueiredo de Almeida
    Laboratorio 02 */

/*  Funcionamento: o algoritmo coloca primeiramente os vertices inicias na fila
  de prioridade e, a partir deles, acessa seus vizinhos também sao colocados na
  fila. O primeiro pixel da fila (o menos custoso) é retirado e verificado se
  ele ja nao foi colocado em alguma arvore. Se ele nao foi, ele é marcado pela
  flag e entao tem seu custo adicionado na solucao. O algoritmo segue assim,
  ate que todos os pixels tenham sido visitados e tenham seu custo computado.
  O algoritmo funciona porque todos os pixels sao visitados, ja que a cada no,
  todos os seus vizinhos sao verificados. Os pixels sao visitados exatamente uma
  vez, ja que na primeira visita eles sao marcados na matriz de visitados e nao
  podem ser mais escolhidos. As arvores que sao percorridas pela algoritmo sao
  de custo minino porque na fila de prioridade sao pegos sempre os pixels mais
  baratos que ainda nao foram visitados.
    Complexidade: as funções cost() e getCost() tem custos lineares. A função
  getNeighborhood() tem operações lineares, com excessão de queue.push(), que,
  por ser uma inserção no heap, tem custo log(t), com t como numero de elementos
  na fila. O pior caso seria em que todos as arestas fossem inseridas, com t =
  O(n*m). getNeighborhood() então tem tempo O(log(n*m)). Todas as operações em
  mst_image() são lineares no tamanho da entrada, com excessão dos loops
  for(auto point : points) que é O(n*m) no pior caso e
  while(queue.empty() == false), que vai rodar para todos os elementos na pilha,
  com pior caso como também O(n*m). Como |V| = n*m, a complexidade é de O(V).*/

#include "solve.hpp"


// Função que calcula o custo entre dois pontos da imagem.
int cost(vector< vector<int> > &image, pair<int, int> a, pair<int, int> b){

    //Se os pontos não forem vizinhos, retorne INT_MAX
    if( abs(a.first - b.first) + abs(a.second - b.second) > 1)
        return INT_MAX;
    //Se forem pontos vizinhos, retorne o módulo da subtração dos pontos
    return abs(image[a.first][a.second] - image[b.first][b.second]);
}

// Pega o custo entre dois pixels e retorna um pixel
pixel getCost(vector< vector<int> > &image, pair<int, int> ponto, pair<int, int> p){
    pixel d;
    d.i = p.first;
    d.j = p.second;
    d.cost = cost(image, ponto, p);
    return d;
}

// Faz o calculo dos custos entres um pixel e seus vizinhos
void getNeighborhood(vector< vector<int> > &image, vector< vector<int> > &choosed, int i, int j,
              priority_queue<pixel, vector<pixel>, comp> &queue, int n, int m, int color){

    // Ja foi escolhido para uma arvore e recebe a cor da raiz
    choosed[i][j] = color;

    // declara os pares de coordenadas da vizinhança
    vector<pair<int, int>> neighborhood {
       make_pair(i - 1, j),
       make_pair(i + 1, j),
       make_pair(i, j + 1),
       make_pair(i, j - 1)
     };

    // pega as coordenadas da raiz
    pair<int, int> root = make_pair(i, j);

    // para cada vizinho, verifica se ele esta dentro dos limites, se nao ja
    // nao faz parte de uma componente e calcula o custo
    for (auto neighbor : neighborhood){
      int i = neighbor.first;
      int j = neighbor.second;
      if ((i >= 0 && i < m) && (j >= 0 && j < n) && choosed[i][j] == -1)
        queue.push(getCost(image, root, neighbor));
    }
}

//Retorne um long long int correspondente ao custo total dos componentes.
long long int mst_image(vector< vector<int> > &image, vector< pair<int, int> > &points){
    long long int solution = 0;

    // dimensoes da imagem
    int m = image.size();
    int n = image[0].size();

    // fila de prioridade com os pixels visitados, como um min-heap
    priority_queue <pixel, vector<pixel>, comp>  queue;

    /* matriz auxiliar que diz se um pixel ja foi escolhido para uma componente
       se == -1, não foi escolhido, se nao ja foi escolhido e esta com a mesma cor
       da raiz */
    vector< vector<int> > choosed(m, vector<int>(n));
    for (vector<int> &row : choosed){
      for (int &val : row)
        val = -1;
    }

    // para cada ponto em points, cria um pixel e coloca na fila com custo zero
    for(auto point : points){
      pixel aux;
      aux.i = point.first;
      aux.j = point.second;
      aux.cost = 0;
      queue.push(aux);
    }

    /* enquanto tiver algum pixel na pilha, pega ele e verifica se ele ja nao
       parte de alguma componente; se ele nao fizer, coloca o custo dele na
       solução e verifica seus vizinhos. tambem envia a cor do pixel atual */
    while (queue.empty() == false){
        pixel p = queue.top();
        queue.pop();
        if (choosed[p.i][p.j] == -1){
          solution += p.cost;
          getNeighborhood(image, choosed, p.i, p.j, queue, n, m, image[p.i][p.j]);
        }
    }
    return solution;
}
