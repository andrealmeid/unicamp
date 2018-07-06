
package Gomoku;

import java.awt.*;
import java.awt.GridBagLayout;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;

public class Visual {

  private JTable tabuleiro;
  private JLabel statusLabel;
  private JButton load;
  private JButton save;
  private JFrame window;
  private Controller controller = new Controller();

  static final int SIZE = 15;

  private void preparaJanela(){
    window = new JFrame("Gomoku");
    window.setSize(270,350);
    window.setLocationRelativeTo(null);

    //faz o programa acaabar quando clicar no 'x'
    window.addWindowListener(new WindowAdapter() {
      public void windowClosing(WindowEvent windowEvent){
        System.exit(0);
      }
    });
  }

  private void preparaTabuleiro(){
    //tabuleiro
    String[][] t = new String[SIZE][SIZE];

    String[] s = new String[SIZE];
    final boolean[] b = new boolean[SIZE];

    tabuleiro = new JTable();
    tabuleiro.setModel(new javax.swing.table.DefaultTableModel(t, s){
        public boolean isCellEditable(int rowIndex, int columnIndex) {
            return b [columnIndex];
        }
    });

    tabuleiro.setTableHeader(null);
    tabuleiro.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);

    //centraliza os itens da tabela
    DefaultTableCellRenderer render = new DefaultTableCellRenderer();
    render.setHorizontalAlignment( JLabel.CENTER );
    for(int i = 0; i < SIZE; i++)
      tabuleiro.getColumnModel().getColumn(i).setCellRenderer( render );

    tabuleiro.addMouseListener(new java.awt.event.MouseAdapter() {
        public void mouseClicked(java.awt.event.MouseEvent evt) {
            tabuleiroMouseClicked();
        }
    });

  }

  private void preparaBotoes(){
        load = new JButton("Load");
        save = new JButton("Save");

        load.addActionListener(new ActionListener(){
          public void actionPerformed(ActionEvent e){
            loadAction();
          }
        });

        save.addActionListener(new ActionListener(){
          public void actionPerformed(ActionEvent e){
            saveAction();
          }
        });
  }

  private void preparaPanel(){
    //prepara o layout da interface
    JScrollPane pane = new JScrollPane(tabuleiro);
    JPanel panel = new JPanel();
    GridBagLayout layout = new GridBagLayout();
    panel.setLayout(layout);

    GridBagConstraints gbc = new GridBagConstraints();
    gbc.fill = GridBagConstraints.BOTH;
    gbc.gridx = 0;
    gbc.gridy = 0;
    gbc.gridwidth = 2;
    panel.add(tabuleiro, gbc);

    gbc.ipady = 20;
    gbc.gridy = 1;
    panel.add(statusLabel, gbc);

    gbc.fill = GridBagConstraints.HORIZONTAL;
    gbc.weightx = 1;
    gbc.ipadx = SIZE;
    gbc.ipady = 20;
    gbc.gridy = 2;
    gbc.gridwidth = 1;
    panel.add(load, gbc);

    gbc.gridx = 1;
    panel.add(save, gbc);

    window.add(panel);
  }

  public void startVisual(){
    preparaJanela();
    preparaTabuleiro();
    preparaBotoes();

    //status
    statusLabel = new JLabel("Jogador(a) atual: 1", JLabel.CENTER);

    preparaPanel();

    window.setVisible(true);
  }

  private void tabuleiroMouseClicked() {
      int i = tabuleiro.getSelectedRow();
      int j = tabuleiro.getSelectedColumn();
      int v = controller.fazJogada(i, j);
      if(v == 0)
        JOptionPane.showMessageDialog(null, "Jogada invalida!");
      else if(v == -1) //em caso de vitoria
        return;
      else if(v == 1 || v == 2)
        printaJogada(v, i, j);
      statusLabel.setText("Jogador(a) atual: " + controller.jogador);
  }

  private void saveAction(){
    JFileChooser jc = new JFileChooser();
    int a = jc.showOpenDialog(null);
    if(a == JFileChooser.APPROVE_OPTION)
      controller.saveGame(jc.getSelectedFile().getPath());
  }

  private void loadAction(){
    JFileChooser jc = new JFileChooser();
    int a = jc.showOpenDialog(null);
    if(a == JFileChooser.APPROVE_OPTION)
      controller.loadGame(jc.getSelectedFile().getPath());
  }

  public void atualizaTabuleiro(int[][] t){
    for(int i = 0; i < SIZE; i++)
      for(int j = 0; j < SIZE; j++)
        printaJogada(t[i][j], i, j);
  }

  public void printaVitoria(int j){
    JOptionPane.showMessageDialog(null, "Jogador(a) " + j + " ganhou!");
  }

  public void printaJogada(int v, int i, int j){
    if (v == 1)
      tabuleiro.setValueAt('X', i, j);
    else if(v == 2)
      tabuleiro.setValueAt('O', i, j);
    else
      tabuleiro.setValueAt(' ', i, j);
  }

  public void printaEmpate(){
    JOptionPane.showMessageDialog(null, "Jogo empatado!");
  }
}
