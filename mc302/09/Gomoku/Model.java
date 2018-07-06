package Gomoku;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.PrintWriter;

public class Model implements ModelInterface {

	int[][] tabuleiro = new int[SIZE][SIZE];
	public Controller controller = new Controller();

	@Override
	public void iniciaPartida() {
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				tabuleiro[i][j] = NONE;
	}

	@Override
	public boolean salvaPartida(String nome) {
		PrintWriter writer;
		try {
			writer = new PrintWriter(nome, "UTF-8");
			for(int i = 0; i < SIZE; i++){
				if(i > 0)
					writer.println("");
				for(int j = 0; j < SIZE; j++)
					writer.print(tabuleiro[i][j]);
			}
			writer.close();
			return true;
		} catch (Exception e) {
			return false;
		}
	}

	@Override
	public boolean carregaPartida(String nome) {
		BufferedReader br;
		try {
				br = new BufferedReader(new FileReader(nome));
				String line = br.readLine();

				int i = 0;
				while (line != null) {
						for(int j = 0; j < line.length(); j++)
							tabuleiro[i][j] = line.charAt(j) - '0';
						line = br.readLine();
						i++;
				}
				br.close();
				return true;
		} catch (Exception e) {
				return false;
		}
	}

	@Override
	public boolean joga(int jogador, int x, int y) {
		if(x >= SIZE || y >= SIZE || tabuleiro[x][y] != NONE)
			return false;

		tabuleiro[x][y] = jogador;
		return true;
	}

	@Override
	public int tab(int x, int y) {
		return tabuleiro[x][y];
	}

	@Override
	public int verifica() {

		//cont1 = contador de pecas pretas
		//cont2 = contador de pecas brancas
		int cont1, cont2;

		//Horizontal
		for(int i = 0; i < SIZE; i++){
			cont1 = cont2 = NONE;
			for(int j = 0; j < SIZE; j++){
				if(tabuleiro[i][j] == BLACK){
					cont1++;
					cont2 = NONE;
				}
				else if(tabuleiro[i][j] == WHITE){
					cont2++;
					cont1 = NONE;
				}
				else
					cont1 = cont2 = NONE;

				if(cont1 == LINELENG)
					return BLACK;
				else if(cont2 == LINELENG)
					return WHITE;
			}
		}

		//Vertical
		for(int i = 0; i < SIZE; i++){
		  cont1 = cont2 = NONE;
			for(int j = 0; j < SIZE; j++){
				if(tabuleiro[j][i] == BLACK){
					cont1++;
					cont2 = NONE;
				}
				else if(tabuleiro[j][i] == WHITE){
					cont2++;
					cont1 = NONE;
				}
				else
					cont1 = cont2 = NONE;

				if(cont1 == LINELENG)
					return BLACK;
				else if(cont2 == LINELENG)
				  return WHITE;
			}
		}

		//Diagonal: esq -> dir
		for(int i = 0; i <= SIZE - 5; i++){
	 		for(int j = 0; j <= SIZE - 5; j++){
				//preto
				if(tabuleiro[i][j] == BLACK &&
					tabuleiro[i+1][j+1] == BLACK &&
					tabuleiro[i+2][j+2] == BLACK &&
					tabuleiro[i+3][j+3] == BLACK &&
					tabuleiro[i+4][j+4] == BLACK)
					return BLACK;

				//branco
				if(tabuleiro[i][j] == WHITE &&
					tabuleiro[i+1][j+1] == WHITE &&
					tabuleiro[i+2][j+2] == WHITE &&
					tabuleiro[i+3][j+3] == WHITE &&
					tabuleiro[i+4][j+4] == WHITE)
					return WHITE;
			 }
		}

		//Diagonal: dir -> esq
		for(int i = 0; i <= SIZE - 5; i++){
			for(int j = SIZE-1; j >= 4; j--){
				//preto
				if(tabuleiro[i][j] == BLACK &&
					tabuleiro[i+1][j-1] == BLACK &&
					tabuleiro[i+2][j-2] == BLACK &&
					tabuleiro[i+3][j-3] == BLACK &&
					tabuleiro[i+4][j-4] == BLACK)
					return BLACK;

				//branco
				if(tabuleiro[i][j] == WHITE &&
					tabuleiro[i+1][j-1] == WHITE &&
					tabuleiro[i+2][j-2] == WHITE &&
					tabuleiro[i+3][j-3] == WHITE &&
					tabuleiro[i+4][j-4] == WHITE)
					return WHITE;
			}
		}

		//nenhum
		return NONE;
	}

}
