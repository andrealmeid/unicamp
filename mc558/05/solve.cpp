/*  Andre Figueiredo de Almeida
    RA: 164047

       Funcionamento: o algoritmo implementa um versão adaptada de Edmonds–Karp.
    Ele cria uma lista de adjacencia para os vertices, entao executa uma busca
    em largura a partir do inicio do parque tentando chegar ao final. Ao conseguir
    um caminho valido, ele percorre ele, encontrando o menor fluxo disponivel.
    Ele aplica esse fluxo em todos os vertices do caminho, atualiza as capacidades
    e fluxos. O caminho eh armazenado na forma de uma arvore de precedencia. Quando
    nao houver mais caminhos validos, o programa para e mostra o fluxo que sai do
    vertice inicial. Esse fluxo eh o maximo porque todos os caminhos dos vertices
    vizinhos ja estao saturados de fluxo ate a atracao final.
        Complexidade: a funcao upadateFlow tem complexida O(|V|). A funcao getMaxFlow
    tem a complexida de uma BFS implementada usando uma lista de adjacencia
    O(|V|+|E|) mais a complexida da funcao updateFlow. A funcao solve tem custo
    de O(|V|) para construir os vetores de fluxo e capacidade e custo O(|V|²) para
    construir a lista de adjacencia. As chamadas para a funcao getMaxFlow tem a
    complexida de Edmonds–Karp, O(|V|*|E|²), que eh a complexida assintotica total
    do algoritmo implementado */

#include "solve.hpp"

// pega o menor fluxo do caminho e aplica ele no caminho todo
void updateFlow(vector<int> predecessors, vector< int > &capacity, vector<int> &flow, int last){

    int min = capacity[last];
    int pred = predecessors[last];

    // percorre o vetor de predecessores, andando pelo caminho e pegando o menor fluxo disponivel
    while(pred != -1){
        if (capacity[pred] < min)
            min = capacity[pred];

        pred = predecessors[pred];
    }

    // percorre novamento o caminho, adicionando o menor fluxo nos fluxos e atualizando a capacidade
    pred = predecessors[last];
    while(pred != -1){
        flow[pred] += min;
        capacity[pred] -= min;
        pred = predecessors[pred];
    }
}

bool getMaxFlow(int inicio, int end, vector< int > &capacity, vector< int > &flow, vector< vector<int>> neighborhood, int v){

    // vetor que diz se um vertice teve seus filhos visitados ou nao
    vector<bool> gray(v);
    for(auto v : gray)
        v = false;

    // vetor de predecessores na arvore geradora encontrada pela BFS
    vector<int> predecessors(v);
    for(int i = 0; i < v; i++)
        predecessors[i] = -1;

    // fila de vertices para o BFS
    queue<int> queue;

    // coloca o primeiro vertice na fila
    queue.push(inicio);

    // executa o loop enquanto tiver alguem na fila
    while(!queue.empty()){
        // pega o primeiro da fila
        int front = queue.front();

        // se ele for o vertice do final do caminho, atualiza o fluxo e retorna true
        if(front == end){
            updateFlow(predecessors, capacity, flow, v-1);
            return true;
        }

        queue.pop();
        gray[front] = true;

        // para cada vizinho do vertice
        for(auto neighbor : neighborhood[front]){
            // verifica se ainda tem alguma capacidade disponivel e se ainda nao foi pintado de cinza
            if ((capacity[neighbor] != 0) && !gray[neighbor]) {
                // pinta de cinza, coloca seu pai como predecessor e coloca ele na fila
                gray[neighbor] = true;
                predecessors[neighbor] = front;
                queue.push(neighbor);
            }
          }
    }

    /* se o a fila acabou e nao foi possivle chegar no final, nao existe caminho
       com capacidade disponivel ate o vertice final */
    return false;
}

int solve(int v, vector<atracao> atracoes){

    // preenche as capacidade com as capacidades dos vertices
    // se for -1, capacidade "infinita"
    vector<int> capacity(v);
    for(int i = 0; i < v; i++){
        if (atracoes[i].repeticoes != -1)
            capacity[i] = atracoes[i].repeticoes;
        else
            capacity[i] = INF;
    }

    // inicializa os fluxos zerados
    vector< int > flow(v);
    for(int i = 0; i < v; i++)
        flow[i] = 0;

    // cria uma matrix de adjacencia
    vector< vector<bool>> neighborhood_matrix(v);

    for(int i = 0; i < v; i++){
      for(int j = 0; j < v; j++){
        neighborhood_matrix[i].push_back(true);
      }
      neighborhood_matrix[i][i] = false;
    }

    // remove os vertices que nao contem arestas a partir de capacidade[i]
    for(int i = 0; i < v; i++){
      for(auto j : atracoes[i].indexNaoSeq){
        neighborhood_matrix[i][j] = false;
      }
    }

    // converte a matrix de adjacencia para lista de adjacencias
    vector< vector<int>> neighborhood(v);
    for(int i = 0; i < v; i++){
        for(int j = 0; j < v; j++){
            if (neighborhood_matrix[i][j])
                neighborhood[i].push_back(j);
        }
    }

    // tenta achar um novo caminho de fluxo da primeira ate a ultima atracao
    while(getMaxFlow(0, v-1, capacity, flow, neighborhood, v));

    // retorna o fluxo que sai do primeiro vertice
    return flow[0];
}
