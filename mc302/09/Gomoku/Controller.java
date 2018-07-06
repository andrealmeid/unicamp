package Gomoku;

public class Controller {

  static Model model = new Model();
  static Visual visual = new Visual();
  public int jogador = 1;
  private int jogadas = 0;

  public static void main(String[] args){
    model.iniciaPartida();
    visual.startVisual();
  }

  public int fazJogada(int x, int y){
    if(model.joga(jogador, x, y)){ //se a jogada for valida
      if(model.verifica() == 0){ //se ninguem ganhou
        if(empate()==0) //verifica empate
          return mudaJogador();
        else
          return -1;
      }
      else
        return ganhou(x, y);
    } else
      return 0;
  }

  public int mudaJogador(){
    if(jogador == 1){
      jogador = 2;
      return 1;
    } else {
      jogador = 1;
      return 2;
    }
  }

  public int empate(){
    jogadas++;
    if(jogadas == 15*15){
      visual.printaEmpate();
      restart();
      return 1;
    }
    return 0;
  }

  public int ganhou(int x, int y){
    visual.printaJogada(jogador, x, y);
    visual.printaVitoria(jogador);
    restart();
    return -1;
  }

  public void restart(){
    jogador = 1;
    model.iniciaPartida();
    visual.atualizaTabuleiro(model.tabuleiro);
    jogadas = 0;
  }

  public void loadGame(String path){
    if(model.carregaPartida(path))
      visual.atualizaTabuleiro(model.tabuleiro);
  }

  public boolean saveGame(String path){
    return model.salvaPartida(path);
  }

}
