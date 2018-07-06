#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack> 
#include "solve.hpp"

/* Inserir Relatorio AQUI */

using namespace std;

int main()
{

	unordered_map<int, string> idToEmail;
	unordered_map<int, string> idToUser;
	vector< vector<int> > userEmail;
	int n,m;

	cin >> n >> m; //Lendo parametros N e M

	//Lendo Nome dos usuarios.
	string tempS;  //variavel auxiliar
	for(int i = 0 ; i < n ; i++ ){
	
		cin >> tempS;  //nome usuario
		idToUser[i] = tempS; 
	}

	//Lendo  Emails.
	for(int i = 0 ; i < m ; i++ ){
	
		cin >> tempS;
		idToEmail[i] = tempS;
	}
	
	//Lendo os Emails de cada usuario
	userEmail.resize(n); 

	int p, e; //Variaveis auxiliares 

	for(int i = 0 ; i < n ; i++){
		
		cin >> p; //numero de emails do usuario
	   	for(int j = 0 ; j < p ; j++) {

			cin >> e; //id do email
			userEmail[i].push_back(e); 
		}		
	}
	
	//Criando vetores que vao receber a solucao
	vector< vector<int> > userSolution;	

	//Chamando funcao solucao
	solve(n, m, userEmail, userSolution);

	//Apresentando numero de pessoas encontrados
	cout << userSolution.size() << endl;
	
	//Apresentando pessoas
	for(int i = 0 ; i < userSolution.size() ; i++){
		
		//apresentando usuarios
		for(int j = 0 ; j < userSolution[i].size() ; j++ ){
		
			cout << idToUser[ userSolution[i][j] ] << " ";
		}
		//Quebra de linha
		cout << endl;
	}	
			
	return 0;
}
