## MC404 -- Tarefa 04 -- Calculadora binária

Nesta tarefa você deve construir uma calculadora binária. Para simplificar, a calculadora vai ter apenas uma operação, de multiplicação. E para simplificar ainda mais, o valor inicial da calculadora deve ser 1.

### Parte A
Escreva uma função de nome _multiplica_ com dois parâmetros (números inteiros sem sinal, 32 bits), passados pela pilha, que retorna em `r0` o valor resultante da multiplicação dos dois parâmetros passados. Considere que os parâmetros são valores relativamente baixos que não causarão estouro de campo do resultado.

Dica: tem uma função quase pronta nas notas de aula! (mas a passagem dos parâmetros é por registrador).

### Parte B
Escreva uma função de nome read, sem parâmetros, que espera o usuário teclar um número hexadecimal de dois dígitos, seguidos da tecla * (ou da tecla '#', veja a seguir), e retorna em `r0` o valor do número hexadecimal teclado e em `r1` o valor da última tecla pressionada (correspondente a '#' ou * ). Por exemplo, se o usuário pressionar a tecla "3" seguida da tecla "8" seguida da tecla * , a função deve retornar em `r0` o valor `0x38` e em `r1` o valor 10 (o teclado devolve esse valor para a tecla * ). Se o o usuário pressionar a tecla "0" seguida da tecla "5" seguida da tecla "#", a função deve retornar em `r0` o valor `0x05` e em `r1` o valor 11 (o teclado devolve esse valor para a tecla '#').

Você pode considerar que o usuário sempre vai digitar dois dígitos e depois a tecla * ou a tecla '#', e que não ocorrerá erro de atropelamento (overrun) no teclado (ou seja, o usuário é lento para digitar, seu programa vai conseguir tratar a tecla antes que outra seja pressionada). Note que, como o teclado não tem teclas para os dígitos hexadecimais de 'a' a 'f', a calculadora somente conseguirá multiplicar números hexadecimais que contenham apenas os dígitos de '0' a '9'!

O dispositivo teclado deve usar as portas 0x80 (porta de dados) e 0x81 (porta de estado). Não vimos ainda "interrupções", portanto esse parâmetro do dispositivo deve ser 0x00. Por exemplo, uma descrição possível do dispositivo teclado para o simulador é:
```
%keyboard CalcPlus
0x80 0x81 0x00
```

### Parte C

Escreva um programa para construir uma calculadora binária, que efetua apenas operações de multiplicação.

Um painel de oito leds, conectado à porta `0x40`, deve ser usado para mostrar o resultado. Inicialmente, o painel de leds deve conter o valor inicial da calculadora, que é 1. Por exemplo, a seguinte descrição pode ser usada para definir o painel de oito leds para o simulador:
```
%leds Resultado
rrrrrrrr 0x40
```
Ao iniciar o programa, o painel de resultado deve mostrar o valor 1. Esse é o valor corrente da calculadora. O usuário deve então digitar um número de dois dígitos seguido da tecla * ou da tecla '#' (seu programa deve usar a função read). A calculadora deve efetuar a multiplicação do valor corrente com o valor digitado, atualizar o valor corrente com o valor do resultado (seu programa deve utilizar a função multiplica), e atualizar o painel de leds com o resultado. A calculadora deve então esperar que o usuário digite um novo valor de dois dígitos, seguido da tecla * , e o processo de multiplicação continua até que o usuário digite dois dígitos seguidos da tecla '#'.

Seu programa deve executar continuamente, efetuando as multiplicações, até o usuário teclar dois dígitos seguidos da tecla '#', quando então seu programa deve parar de executar (use a instrução HLT para parar a execução do programa no simulador).

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo init. Lembre que você tem que reservar espaço para a pilha e inicializar o registrador apontador de pilha (`sp`).

Por razões de testes no Susy, você não deve utilizar a memória abaixo do endereço `0x1000`.
