## MC405 -- Tarefa 05 -- Contador temporizado

Nesta tarefa você deve construir um contador (de tempo) decrescente, que pode ser usado como um temporizador. Para simplificar, o contador vai ter apenas um dígito decimal.

O contador vai ser composto por um teclado e um por um display de sete segmentos. O usuário pode escolher um valor (entre 0 e 9) usando o teclado. O contador mostra o valor escolhido pelo usuário e inicia a contagem regressiva, atualizando o valor no display a cada segundo, até o display mostrar o valor zero. Como veremos, o contador não vai marcar o tempo de forma precisa; veremos no futuro como implementar um temporizador mais preciso.

O contador deve iniciar mostrando o valor zero. Se o usuário digitar a tecla 3, o contador deve mostrar o valor 3 no display, esperar um segundo (mais ou menos!), mostrar o valor 2 no display, esperar um segundo, mostrar o valor 1, esperar um segundo, mostrar o valor 0 e esperar pela próxima entrada do usuário.

Esta tarefa não pode ser feita no Cloud 9, pois este não tem suporte para elementos gráficos como o teclado!

### Parte A
Escreva uma rotina de tratamento de interrupção do teclado, que simplesmente atualize o valor de uma variável de nome `contador` com o valor da tecla pressionada pelo usuário.

Dica: tem uma rotina de tratamento do teclado nas notas de aula!

### Parte B
Escreva um procedimento de nome `mostra`, com um parâmetro (um valor entre 0 e 9), passado pelo registrador `r0` e mostra esse valor no display de sete segmentos.

### Parte C
Escreva um procedimento de nome `decrementa` que recebe em r0 um valor entre 0 e 9, mostra esse valor inicial, espera um certo tempo, decrementa e mostra o novo valor, espera um certo tempo, decrementa e mostra o novo valor, etc, até que o valor seja zero, A espera deve ser feita com um "loop de espera", que apenas gasta tempo, por exemplo inicializando um registrador qualquer com um valor específico e decrementando o valor desse registrador até que seja igual a zero.

Você deve ler o valor específico usado na espera de uma variável de nome `intervalo`. (Obs.: no meu laptop, para conseguir um tempo de mais ou menos um segundo usando um loop simples como o descrito acima, essa variável deve ser inicializada com o valor `0x400000`. No Susy, o valor dessa variável vai ser bem mais baixo, para executar mais rápido, já que esse loop de espera apenas gasta tempo, mas para verificar visualmente ele tem que ser alto).

O dispositivo teclado deve usar as portas 0x80 (porta de dados) e 0x81 (porta de estado), e deve usar a interrupção de tipo 0x10. Você deve obrigatoriamente usar interrupções, caso contrário sua nota será drasticamente reduzida! Note que, usando interrupções, você não precisa consultar a porta de estado do teclado, a não ser que queira tratar erros de atropelamento (mas não precisa!).

Por exemplo, uma descrição possível do dispositivo teclado para o simulador é:
```
%keyboard Entrada
0x80 0x81 0x10
```
e uma descrição possível do display de sete segmentos é:
```
%7segs  Contador
0x20
```

### Parte D
Escreva um programa para construir o Contador temporizador, usando as funções definidas acima.

Seu programa deve executar continuamente, efetuando a temporização com o valor inicial digitado pelo usuário no teclado. Seu programa deve escrever no display de sete segmentos o menor número de valores possíveis para que o temporizador esteja correto. Em outras palavras, você não deve escrever no display o valor que já está no display. Por exemplo, se o display mostra o valor 0, você não deve escrever o valor correspondente a 0 novamente na porta do display.

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo init. Lembre que você tem que reservar espaço para a pilha, inicializar o registrador apontador de pilha (`sp`), inicializar o vetor de interrupções e habilitar interrupções (com a instrução `sti`).

Por razões de testes no Susy, você não deve utilizar a memória abaixo do endereço 0x1000.

### Simplificações
Você pode assumir que:
- O usuário não vai pressionar qualquer tecla enquanto o temporizador estiver contando.
- O usuário sempre vai digitar apenas um dígito, e esperar o temporizador contar.
- O usuário não vai pressionar as teclas * ou '#'.
