
## MC404 -- Tarefa 01 -- Invertendo uma sequência de caracteres

Escreva um programa em linguagem de montagem LEG para inverter uma dada cadeia de caracteres ASCII. Por exemplo, se a cadeia de entrada é

`ABCDEF1234`

o seu programa deve produzir a cadeia

`4321FEDCBA`

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo start. Ao terminar de inverter a cadeia, seu programa deve executar a instrução HLT (interrompe processamento).

### Entrada
A cadeia de caracteres a ser invertida inicia no endereço 0x1000. Um byte com o valor 0x00 indica o final da cadeia (e não deve ser incluído na inversão!). A cadeia contém no máximo 15 caracteres e no mínimo um caractere (não contando o byte indicador de final).

### Saída
A cadeia de caracteres invertida deve iniciar também no endereço 0x1000 (ou seja, a cadeia deve ser invertida no mesmo lugar em que é dada).
