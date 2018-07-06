## MC404 -- Tarefa 02 -- Soma de vetores

São fornecidos

- um vetor de inteiros sem sinal, de 16 bits, com elementos armazenados consecutivamente (dois elementos por palavra) a partir do endereço `0x108`, de rótulo `vetor`;
- um valor que é uma potência de 2 (entre 2,4,8), armazenado numa palavra de memória (32 bits) cujo rótulo é `divisor`, e endereço `0x100`.
- O número de elementos do vetor, dado em uma palavra de memória (32 bits) cujo rótulo é `num_elem` (valor entre 1 e 16), cujo endereço é `0x104`.

Escreva um programa em linguagem de montagem LEG para calcular a soma dos elementos de `vetor` divididos pelo valor de `divisor` (ou seja, cada elemento deve ser dividido por `divisor` e o resultado deve ser a soma de todos esses valores). O resultado deve ser colocado no registrador `r0`.

Por exemplo, se `vetor` tem inicialmente os valores [5,12,1,16,3,27,0] e o valor de `divisor` é 4, ao final da execução de seu programa r0 deve conter o valor 14 (soma dos valores [1,3,0,4,0,6,0]).

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo `init`. Ao terminar seu programa deve executar a instrução HLT (interrompe processamento).
