
## MC404 -- Tarefa 03 -- Soma de vetor de 64 bits

### Parte A

Escreva uma função de nome `add64` com dois parâmetros, que são números inteiros sem sinal de 64 bits, passados pela pilha, e calcule a soma desses dois números, retornando o resultado nos registradores `r1` (palavra mais significativa do resultado) e `r0` (palavra menos significativa do resultado). Sua função pode desconsiderar erros de estouro de campo.

Cada parâmetro de 64 bits é empilhado da seguinte forma: primeiro é empilhada a palavra (32 bits) mais significativa, depois a palavra (32 bits) menos significativa do parâmetro.

Por exemplo, se os argumentos da função são
`0x11111111AAAAAAAA` e `0x22222222AAAAAAAA`

respectivamente, em decimal, `1229782940824283818` e `2459565878785256106`, a pilha no momento da execução da função é:
```
|          |
------------
| 11111111 |
------------
| AAAAAAAA |
------------
| 22222222 |
------------
| AAAAAAAA | <-- sp
------------
|          |
```
e o resultado deve ser
`r1=0x33333334` e `r0=0x55555554`

Por razões de testes no Susy, o início da função (rótulo `add64`) deve estar no endereço 0x1000.

### Parte B
Escreva um trecho de programa, cujo endereço de início é `0x2000` e tem rótulo `init`, que calcule a soma dos elementos de um vetor de números de 64 bits, deixando o resultado nos registradores `r1` (parte mais significativa do resultado) e `r0` (parte menos significativa do resultado). O vetor, com um máximo de 100 elementos, está armazenado a partir do endereço `0x104`, de rótulo `vetor`. O número de elementos está armazenado em uma palavra de 32 bits de rótulo `num_elem` (inteiro sem sinal), no endereço `0x100`.
Você pode assumir que o número de elementos do vetor é maior ou igual a 1.

Assuma que na memória, um número de 64 bit é armazenado da seguinte forma:

- a palavra menos significativa (32 bits) é armazenada no endereço X e
- a palavra mais significativa (32 bits) é armazenada no endereço X+4.

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo init. Lembre que você tem que reservar espaço para a pilha e inicializar o registrador apontador de pilha (`sp`).

Ao terminar seu programa deve executar a instrução HLT (interrompe processamento).
