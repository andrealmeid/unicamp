# Problema da Mochila Relacional
## André Almeida _164047_


## O problema
A entrada do problema é dada por:
- Número de itens;
- Matriz de relações de itens;
- Vetor de valor de itens;
- Vetor de peso de itens;
- A capacidade da mochila e
- O tempo máximo disponível para o cálculo da solução

Todos os resultados mostrandos foram executados em uma máquina com processador Intel i7 de 3ª geração.

## Algoritmo exato

A abordagem exata para o problema foi criar uma estrutura de solução global, com a soma dos itens e um vetor de índices de itens da solução. Esta estrutura é atualizada sempre que é encontrada uma solução melhor. Então, o proximo passo é ordenar os elementos segundo o cálculo do valor relativo¹ deles e criar soluções com apenas um item, começando pelo de maior valor relativo. Se houver espaço na mochila, chamamos uma rotina para inserir itens um por um, verificando se gera uma solução melhor e, se ainda houver espaço na mochila, chama a rotina recursivamente para inserir mais um elemento. Quando não há mais espaço não chamamos a rotina, e desta forma cortamos caminhos que não seriam frutíferos. Por exemplo, se um conjunto de itens `[a, b, c]` já ocupa a capacidade máxima da mochila, nenhum item será capaz de ocupar a mochila e não faz sentido calcularmos soluções com o conjunto inicial `[a, b, c]`. O algoritmo não gera combinações repetidas nem permutações, porque sempre que for inserir um índice de item, seleciona um de índice maior que o seu. Por exemplo, para uma entrada com 4 itens, o algoritmo irá gerar uma árvore assim:

![tree](/assets/tree.png)

O algoritmo pega cada vetor (nó) da árvore e percorre os índices, verificando a solução que estes índices geram. Se a capacidade estiver cheia, o vetor atual vira uma folha.

¹O _valor relativo_ é calculado fazendo a soma do valor de um item mais todas os valores de suas relações e dividindo pelo seu peso:
> valor_relativo = (somaTodasRelações(item) + valor[item]) / peso[item]


### Implementação
Em detalhes a implementação do algoritmo é feita da seguinte maneira, começando pela rotina principal:
> _1._ Ordenar decrescentemente os `n` itens pelo seu _valor relativo_ e iniciar o alarme com o tempo máximo
  _2._ Criar uma estrutura global `solução`, com a soma igual à 0 e o conjunto de itens vazio
  _3._ Chamar a rotina `calculaSolução(items, valores, pesos, relações, capacidade)`
  _4._ Quando a rotina acabar ou quando o alarme acabar, retorna o conjunto de items e a soma da solução atual


A rotina `calculaSolução(...)` por sua vez é definida por:

> _1._ Pegar os itens ordenados e, para cada índice de item `i`:
  _2._ &nbsp; &nbsp;  Verificar se o item cabe na mochila e, se couber:
  _3._ &nbsp; &nbsp; &nbsp; &nbsp; Cria uma solução com apenas este elemento e compara com a solução global. Se for melhor, substituiu ela.
  _4._ &nbsp; &nbsp; &nbsp; &nbsp; Se ainda houver espaço na mochila, chama a rotina `calculaFilhos([i], valor[i], capacidade - v[i])`

Nessa rotina `calculaSolução(...)` criamos soluções com apenas um item e, caso ainda exista espaço disponível, chamamos uma subrotina para verificar as combinações que são "filhas" desta rotina, de forma a percorrer todas as combinações que são válidas. Usamos um vetor ordenado para, no caso em que o tempo estourar, ter uma chance que tenhamos começados por itens que venham a ter soluções mais próximas da ótima. Para cada `i`, passamos o `valor[i]` como uma soma parcial e reduzimos a capacidade da mochila subtraindo o peso de `i`. A rotina `calculaFilhos(itens[], soma_parcial, capacidade)`  funciona de maneira semelhante, mas sem a etapa de buscar os itens ordenados de forma a evitar consultas a vetor que iriam pesar no algoritmo.

Rotina `calculaFilhos(itens[], soma_parcial, capacidade)`:
> _1._ Para `j`, começando no último índice de `itens[] + 1` até `número total de itens`:
  _2._ Se o alarme esgotou, retornamos a solução global do momento
  _3._ Se o `peso[j]` cabe na mochila:
  _4._ &nbsp; &nbsp; Adiciona o `j` no vetor `itens`
  _5._ &nbsp; &nbsp; Calculamos a `soma_atual`, somando a `soma_parcial` + `valor[j]` + o valor das relações de `j` com os itens maiores que `j` que já estão na solução
  _6._ &nbsp; &nbsp; Se esta soma for maior do que a solução global, substituiu a global por ela
  _7._ &nbsp; &nbsp; Se ainda existe espaço na mochila:
  _8._ &nbsp; &nbsp; &nbsp; &nbsp; Chamamos `calculaFilhos(itens[], soma_atual, capacidade - peso[j])`
  _9._ &nbsp; &nbsp; Retiramos `j` do vetor `itens`

O calculo na linha _5_ é feito tomando cuidado para não somar duas vezes a mesma relação. Como os itens são inseridos de maneira ordenada, isto é, o índice de um novo item inserido será maior que qualquer índice que já esteja na solução, basta somar as relações começando do índice do item novo.

### Resultados

#### Casos pequenos
Para os casos de teste de 01 à 12, o algoritmo exato executou abaixo de um segundo e obteve a resposta correta

## Algoritmo aproximado
