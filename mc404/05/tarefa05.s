@ tarefa05 - Contador temporizador
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    KEYBD_DATA   .equ 0x80  @ porta de dados
    INT_KEYBD    .equ 0x10  @ tipo de interrupcao do teclado
    DISPLAY_DATA .equ 0x20  @ porta de saida do display

    .org INT_KEYBD * 4      @ vetor de interrupcao
    .word trata_int_teclado

    .org 0x1000

fim_pilha:
    .skip  12
ini_pilha:

init:
    sti                     @ habilita interrupcoes
    set sp, ini_pilha       @ inicializa a pilha
    set r0, 0x7e
    outb DISPLAY_DATA, r0   @ inicializa o display com 0
    set r0, 0               @ inicializa o r0 como 0
    stb contador, r0

main_loop:
    ldb r0, contador        @ carrega o valor do contador
    cmp r0, 0
    jnz muda_display        @ se r0 mudou, chama o a rotina que altera o display
    jmp main_loop           @ senao, continua o loop principal

muda_display:
    push r0                 @ envia r0 como parametro
    call decrementa         @ chama a rotina de decremento
    pop r0
    jmp main_loop           @ volta pro loop principal

decrementa:
    ldb r0, [sp+4]          @ pega o numero inicial da contagem

decrementa_loop:
    push r0
    call mostra             @ exibe o numero atual
    pop r0
    ld  r1, intervalo       @ aguarda um intervalo de tempo

loop_espera:
    cmp r1, 0               @ se o tempo for 0, acabou o intervalo
    jz end_loop_espera
    sub r1, 1               @ se nao, subtrai 1
    jmp loop_espera

end_loop_espera:
    cmp r0, 0               @ se o numero for 0, acabou a contagem
    jz decrementa_end
    sub r0, 1               @ se nao, subtrai 1
    jmp decrementa_loop

decrementa_end:
    stb contador, r0        @ atualiza a variavel do contador
    ret

mostra:
    ldb r0, [sp+4]          @ pega o numero a ser exibido
    set r1, tab_digitos     @ coloca r1 no vetor de digitos
    add r0, r1              @ r0+r1, de forma que r0 tenha o digito correto
    ldb r0, [r0]            @ carrega o digito atual
    outb DISPLAY_DATA, r0   @ exibe no display
    ret

trata_int_teclado:
    push  r0                @ preserva valor dos registradores
    inb   r0, KEYBD_DATA    @ le porta de dados
    stb   contador, r0      @ armazena valor lido
    pop   r0                @ restaura registrador
    iret

intervalo:
    .word 0x380000

contador:
    .skip 1
tab_digitos:
    .byte  0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b,0x4f,0x4f
