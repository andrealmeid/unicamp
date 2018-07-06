
## MC404 -- Tarefa 06 -- Tudo em Maiúsculas

Escreva um programa que leia do console uma cadeia de caracteres ASCII, com no máximo 256 caracteres, transforme as letras minúsculas da cadeia em maiúsculas, e escreva o resultado de volta no console. Ao final o registrador `r10` deve conter o número de letras que foram alteradas.

Para escrever no console seu programa deve usar as chamadas ao sistema `read` e `write`. Consulte no manual de dispositivos a forma de utilização do console.

Para a chamada `read` o vetor onde devem ser armazenados os bytes lidos deve estar obrigatoriamente no endereço 0x2000.

Ao término da execução, seu programa deve usar a chamada ao sistema `exit(0)`.

A primeira instrução a ser executada pelo seu programa deve estar no endereço de rótulo init. Lembre que você tem que reservar espaço para a pilha, inicializar o registrador apontador de pilha (`sp`).

Por razões de testes no Susy, você não deve utilizar a memória abaixo do endereço 0x1000.

### Exemplo
Se a cadeia de entrada for

`Meu burro custou R$ 100,00, mas nao o vendo por dinheiro algum.`

a saída no console deve ser

`MEU BURRO CUSTOU R$ 100,00, MAS NAO O VENDO POR DINHEIRO ALGUM.`

e o registrador `r10` deve conter o valor `0x29` (41 em decimal).

Note que as letras na entrada não são acentuadas (a codificação é ASCII).

### Entrada
A entrada é feita pelo console.

### Saída
A saída é feita através do console e do registrador `r10`.

Esta tarefa não pode ser feita no Cloud 9, pois este não tem suporte para elementos gráficos como o console!

### Dicas
- O parâmetro r0 da chamada ao sistema exit deve ser igual a zero, para indicar que o programa terminou de executar sem erros.
- Note que a diferença na representação ASCII entre uma letra maiúscula e uma minúscula é apenas um bit; portanto basta trocar o valor desse bit para transformar uma letra de maiúscula para minúscula (e vice-versa).
- Veja nas notas de aula um exemplo de programa que usa chamada a sistema.
- Note que é necessáro usar a opção "-c" do legsim para "caregar" o dispositivo console na simulação (legsim -c).
