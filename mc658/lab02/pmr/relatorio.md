<center>
<h1> Problema da Mochila Relacional com PLI</h1>
<h2> André Almeida <i>164047</i>
<br>MC658 - Lab 04</h2>
</center>

## I. O problema

São dados $n$ itens, $N = {1, ..., n}$, e uma mochila de capacidade $C$. Cada item $i \in N$ tem um peso $s_i > 0$ e valor $v_i$ e cada par de itens $i, j \in N$ tem uma relação $r_{ij}$ (não necessariamente positiva). O objetivo é encontrar um subconjunto $S \subseteq N$ tal que $\sum\limits_{i \in S} {s_i \leqslant C}$ e $\sum \limits_{i \in S}{v_i}+{\sum\limits_{i \in S} \sum \limits_{j\in S, j>i} r_{ij} }$ é máximo.

A entrada do problema é dada por:
- Número de itens;
- Matriz de relações de itens;
- Vetor de valor de itens;
- Vetor de peso de itens;
- A capacidade da mochila e
- O tempo máximo disponível para o cálculo da solução

Todos os resultados mostrandos foram executados em uma máquina com processador Intel Core i7-3537U CPU @ 2.00GHz, rodando com GNU/Linux 4.15. O compilador usado foi o GCC 7.3.0, e o Gurobi 7.5.2 como o _solver_ do PLI, usando as _flags_ de compilação do Makefile disponibilizado.


## II. Programação Linear Inteira
A programção linear inteira é um jeito otimizado de resolver problemas lineares onde as soluções são números inteiros. O algoritmo que resolve esse sistema faz parte da classe de problemas **NP-Completos**. Para definirmos nosso sistema linear, precisamos definir as varíveis do sistema, o que queremos maximizar/minizar e as restrições as quais nossas variáveis estão sujeitas.

###a. Definição do modelo
Nesse problema, temos duas variáveis:
    - $ x_i$, $i \in N $: define se o objeto $i$ está na solução $S$;
    - $ y_{ij}$,  $i, j \in N, j > i$: define se a relação entre os objetos $i$ e $j$ deve ser aplicada, ou seja, se $i$ e $j$ fazem parte da solução. Fazemos com que $j > i$ para evitar pegar a mesma relação duas vezes.

Queremos então máximizar o valor dos objetos escolhidos mais a soma das relações:

$max \sum \limits_{i \in S}{v_i}+{\sum\limits_{i \in S} \sum \limits_{j\in S, j>i} r_{ij} }$

As nossas restrições são:
- $\sum\limits_{i \in S} {s_i \leqslant C}$: a soma de todos os pesos dos objetos na solução não devem ser maior que a capacidade;
- $2 * y_{ij} \leqslant x_i + x_j$: $y_{ij}$ não deve ser 1 se ambos objetos não estão na solução;
- $1 + y_{ij} \geqslant x_i + x_j$: $y_{ij}$ deve ser 1 se ambos objetos estão na solução;

###b. Resultados
Com estes dados aplicados no código usando o Gurobi, foi possível chegar
nestes resultados (com o tempo máximo definido como 120 segundos):

| Teste   | Precisão (%) | Tempo |
|-----|---------|---------|
| 1   | 100,00% | 0.0142  |
| 2   | 100,00% | .0.112  |
| 3   | 100,00% | 0.0130  |
| 4   | 100,00% | 0.0197  |
| 5   | 100,00% | 0.0215  |
| 6   | 100,00% | 0.0190  |
| 7   | 100,00% | 0.0466  |
| 8   | 100,00% | 0.0571  |
| 9   | 100,00% | 0.0706  |
| 10  | 100,00% | 0.0850  |
| 11  | 100,00% | 0.4466  |
| 12  | 100,00% | 18.1200 |
| 200 | 100,00% | 72.0984 |
| 300 | 5,61%   | 120     |
| 400 | 3,31%   | 120     |
| 500 | 0,79%   | 120     |
| 600 | 1,27%   | 120     |

## III. Heurística

Foi criada no experimento passado uma heurística para resolver o problema de forma aproximada. A heurística ordena decrescentemente os itens pelo valor relativo, definido a seguir:

> valor_relativo = (somaTodasRelações(item) + valor[item]) / peso[item]

Após a ordenação, ela insere o maior item não inserido que cabe na mochila até que ela esteja cheia e toma estes itens como a solução. Essa heurística foi escolhida porque privilegia os itens com maior potencial para contribuirem com uma soma de valores próxima da ótima. A divisão pelo peso garante que o valor relativo cresça conforme o quanto da mochila que irá ocupar.


### a. Implementação
A implementação da heurística é dada por:

> _01._ Inicia o alarme com o tempo máximo e ordena decrescentemente o vetor `itens` com os `n` itens pelo seu _valor relativo¹_
  _02._ Inicializa `soma` como 0
  _04._ Inicializa `capacidade_parcial` com o valor da `capacidade`
  _05._ Inicializa `solucao` como um vetor vazio
  _06._ Para `ì`, de `0` até `n`:
  _07._ &nbsp; &nbsp; Se o alarme acabou, retorna a soma e solução atual
  _08._ &nbsp; &nbsp; Se `peso[i] <= capacidade_parcial`:
  _09._ &nbsp; &nbsp; &nbsp; &nbsp; Coloca `i` em `solucao`
  _10._ &nbsp; &nbsp; &nbsp; &nbsp; Adiciona `valor[i]` em `soma`
  _11._ &nbsp; &nbsp; &nbsp; &nbsp; Adiciona `valorRelacoes(i, solucao)` em `soma`
  _12._ &nbsp; &nbsp; &nbsp; &nbsp; Subtrai `peso[i]` de `capacidade`
  _13._ Retorna `soma` e `solucao`

###b. Resultados
Então, aplicamos o valor encontrado pela heurística como um valor inicial para o Gurobi e desabilitamos o _presolver_ dele, para usar apenas o nosso. Todos os resultados obtiveram 100% de precisão, por isso esse número foi omitido.
