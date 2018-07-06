/* 	Andre Figueiredo de Almeida
	RA: 164047
	MC558 Lab 02 */

/* Funcionamento: o algoritmo percorre o userEmail verificando os emails dos
  usuarios. Nesse cenario, existem duas possibilidades: ou o email Y ja foi
  citado ou ele eh novo. Um vetor guarda o estado dos emails. Se ele ja foi
  citado por algum usuario X, o usuario atual eh vizinho do usuario X e
  vice-versa (isso eh armazenado na estrutura user). Se o email eh inedito,
  o usuario eh salvo no vetor de estado de emails (emailOwner) como "dono" do
  email Y. Depois que os todos usuarios ja conhecem seus vizinhos, o grafo esta
  montado. Entao, uma busca em largura percorre as componentes, determinando um
  ID para cada componente. Os numeros dos usuarios sao transcritos para o
  userSolution de acordo com seus IDs.
   Complexidade: os loops que inicializa a matriz de email tem tempo m, o loop
  que inicializa os usuarios e preenche os emails tem tamanho (n + m). Na busca,
  os vertices sao buscados somente se eles forem brancos, e uma vez pintados de
  pretos, eles nao voltam a ser brancos. Dessa forma, a busca passa apenas uma
  vez pelos n vertices. Como o grafo cria arestras entre os usuarios, a busca
  nao percorre os vertices de emails. O loop que prepera a matriz de solucao
  do usuario tem tamanho n. */


#include "solve.hpp"

void search(vector<user> &users, user *root, int group){

	// define o grupo ao qual esse usuario pertence
	root->group = group;

	// pinta o no de preto (foi visitado e iniciou a busca nos filhos)
	root->color = BLACK;

	// se esse no possui vizinhos nao visitados, faz a busca neles
	if(!root->neighbors.empty()){
		for(int neighbor : root->neighbors){
			if(users[neighbor].color == WHITE)
				search(users, &users[neighbor], group);
		}
	}
}

//Retorne um pair correspondente aos usuarios de cada componente.
void solve(int n, int m, vector< vector<int> > &userEmail, vector< vector<int> > &userSolution){

	// Cria um vetor de emailOwner.
	/* Esse vetor e responsavel por armazenar qual o primeiro usuario a citar
	   o i-esimo email */
	vector<int> emailOwner;

	// Inicializa todos os m donos como -1
	for(int i=0; i < m; i++)
		emailOwner.push_back(-1);

	// Cria um vetor de usuarios
	vector<user> users;

	// Inicializa os n usuarios
	for(int i = 0; i < n; i++){

		user newUser;
		newUser.color = WHITE; // inicialmente nao foram visitados
		users.push_back(newUser);

		// acessas os emails de newUser
		for (int email : userEmail[i]){

			// se ninguem e dono, newUser se torna dono
			if(emailOwner[email] == -1)
				emailOwner[email] = i;

			// se ja existe um dono, adiciona o newUser como vizinho do dono
			// e coloco o emailOwner como vizinho do newUser
			else{
				users[emailOwner[email]].neighbors.push_back(i);
				users[i].neighbors.push_back(emailOwner[email]);
			}
		}
	}

	// Realiza a busca em largura em todos vertices nao visitados
	int num_group = 0;
	for(int i=0; i < n; i++){
		// incrementa o numero do grupo para cada componente nova
		if(users[i].color == WHITE)
			search(users, &users[i], num_group++);
	}

	// redimensiona o vetor de solucoes com o numero de grupos encontrados
	userSolution.resize(num_group);

	// coloca nos vetores de solucoes os membros de cada grupo de acordo com o
	// indice do grupo
	for(int i=0; i<n; i++)
		userSolution[users[i].group].push_back(i);

}
