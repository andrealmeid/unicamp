@ tarefa04 - Calculadora binaria
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    @ constantes de entrada e saida de dados
    KEYBD_DATA  .equ 0x80       @ porta de dados do teclado
    KEYBD_STAT  .equ 0x81       @ porta de estado do teclado
    KEYBD_READY .equ 0x1        @ bit READY
    LEDS        .equ 0x40       @ porta de saida dos leds
    @ constantes
    INPUT_SIZE  .equ 0x3        @ numero de digitos clicados por leitura
    INPUT_ON    .equ 0xA        @ quando usuario aperta o botao *
    INPUT_OFF   .equ 0xB        @ quando usuario aperta o botao #

    .org 0x1000
fim_pilha:
    .skip  0xC
ini_pilha:

multiplica:
    ld    r3, [sp+8]    @ pega valores da pilha
    ld    r4, [sp+4]
    cmp   r3, r4        @ para minimizar os passos da multiplicacao
    jnc   mult1         @ usa menor valor para controlar repeticao
    mov   r0, r3        @ troca valores de r3 e r4 usando
    mov   r3, r4        @ r0 como temporario
    mov   r4, r0
mult1:
    set   r0, 0         @ inicializa valor do produto
mult2:
    sub   r4, 1         @ vamos realizar r4 adicoes
    jc    mult3         @ desvia se terminamos
    add   r0, r3        @ adiciona mais uma parcela
    jmp   mult2
mult3:
    ret                 @ retorna quando todas as adicoes terminaram

read:
    inb r7, KEYBD_STAT  @ recebe o estado do teclado
    tst r7, r6          @ testa com o registrador que guarda KEYBD_READY
    jz  read            @ se nao tem nada para ler, faz um loop
    cmp r5, INPUT_SIZE  @ verifica se o primeiro digito ja foi lido
    jz read_data1
    cmp r5, 2           @ segundo digito
    jz read_data2
    jmp read_data3      @ terceiro e ultimo digito (* ou #)
read_data1:
    inb r0, KEYBD_DATA  @ recebe dado
    shl r0, 4           @ desloca 4 bits para esquerda
    sub r5, 1           @ diminui um no contador da entrada
    jmp read
read_data2:
    inb r1, KEYBD_DATA  @ recebe dado
    add r0, r1          @ adiciona no r0, ja devidamente deslocado
    sub r5, 1           @ diminui um no contador da entrada
    jmp read
read_data3:
    inb r1, KEYBD_DATA  @ sobreescre r1 com caractere de controle (*, #)
    jmp read_end        @ fim da leitura
read_end:
    ret

calc:
    cmp r1, INPUT_OFF   @ verifica se o caractere eh #
    jz  end             @ se sim, finaliza o programa
    call read           @ se nao, chama a rotina de leitura do teclado
    set r5, INPUT_SIZE  @ reinicia o contador da entrada
    push r8             @ envia o valor corrente para pilha
    push r0             @ envia o que foi lido pra pilha
    call multiplica
    mov r8, r0          @ pega o retorno e coloca no valor corrente
    outb LEDS, r8       @ envia o valor para os LEDs
    jmp calc

init:
    set sp, ini_pilha   @ inicia a pilha
    set r8, 0x1         @ registrador do valor corrente
    set r1, INPUT_ON    @ registrador que guarda * ou #; inicia com *
    set r5, INPUT_SIZE  @ registrador do tamanho da entrada (3 digitos)
    set r6, KEYBD_READY @ indica se o teclado tem dado para ser lido
    outb LEDS, r8       @ inicia os LEDs com valor corrente
    call calc           @ chama a rotina de ler o teclado

end:
    hlt
