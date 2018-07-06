package Gomoku;

public interface ModelInterface {

	static final int NONE     = 0;   // nenhum jogador
	static final int BLACK    = 1;   // jogador 'preto'
	static final int WHITE    = 2;   // jogador 'branco'
	static final int GAMEOVER = 3;   // partida encerrada
	static final int LINELENG = 5;   // 5 (em linha)

	static final int SIZE = 15;      // tamanho padrao do tabuleiro


	/**
	 * Inicia nova partida, 'zerando' o tabuleiro
	 */
	public void iniciaPartida();

	/**
	 * Salva a partida atual sob um nome, para continuar depois
	 * @param nome nome sob o qual a partida e salva (em arquivo)
	 */
	public boolean salvaPartida(String nome);

	/**
	 * Restaura a partida atual, para continuar o jogo
	 * @param nome nome sob o qual a partida foi salva (em arquivo)
	 */
	public boolean carregaPartida(String nome);

	/**
	 * Registra uma jogada.
	 * @param jogador
	 * @param x
	 * @param y
	 * @return jogada valida (true) ou invalida (false)
	 */
	public boolean joga(int jogador, int x, int y);

	/**
	 * Retorna a peca na posicao (x,y) :
	 *  (NONE, BLACK, WHITE)
	 * @param x
	 * @param y
	 * @return
	 */
	public int tab(int x, int y);

	/**
	 * Verifica se existe um vencedor
	 * (algum jogador fez '5 em linha')
	 * @return
	 */
    public int verifica();

}
