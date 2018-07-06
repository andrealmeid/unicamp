
#include "solve.hpp"
#include <chrono>

using namespace std;


int main(int argc, char *argv[])
{

  auto start = std::chrono::high_resolution_clock::now();
    string cv;
	int size;
	int n, m, k;

	cin >> cv; // lendo tipo de codigicação
	cin >> m >> n; // parametros de colunas e linhas
	cin >> size; // lendo valor máximo da entrada

    //chegando entrada
    if(size != 255 || cv != "P2"){
        return 1;
    }

	vector< vector<int> > image( n, vector<int> (m) );

	//lendo matriz de pontos da imagem
	for(int i = 0 ; i < n ; i++ )
		for (int j = 0 ; j < m ; j++)
			cin >> image[i][j];

    //parametro de pontos para segmentação
	cin >> k;
	vector< pair<int, int> > points;

	for ( int i = 0 ; i < k ; i++ ){
		int tempX, tempY;
		cin >> tempY >> tempX; // lendo posicao de coluna e linha do ponto base da segmentacao
		points.push_back( make_pair(tempX, tempY) ); //!
	}

	long long int s =  mst_image(image, points); // valor

    /*Descomentando o trecho abaixo sua saida pode ser vista em aplicativos de imagem.
     Basta salvar no formato .pgm, uma forma fácil é redirecionando o output,
     por exemplo executando com o comando ./lab03 <inst1.in >out_inst1.pgm

     As entradas desse lab também podem ser vistas como imagem, para isso é preciso salva-las como .pgm */


     //--------------------------------------------------

	// cout << "P2" << endl;
	// cout << m <<" "<<  n << endl;
	// cout << "255" << endl;
	// for(int i = 0 ; i < n ; i++ ){
	// 	for (int j = 0 ; j < m ; j++){
	// 		printf("%d", (image[i][j] * -1));
	// 		if( j != m -1)
	// 			printf(" ");
	// 	}
	// 	printf("\n");
	// }

    //--------------------------------------------------

  cout << s << endl;

  auto finish = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";


    return 0;
}
