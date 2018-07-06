@ tarefa09 - Semaforos
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    .global _start
    .org 0x9000

    .equ verde, 1
    .equ amarelo, 2
    .equ vermelho, 4

_start:
    mov r0, #0x90000    @ r0 -> endereco do farol 2
    add r0, #1          @ r0 -> 0x90001
    mov r1, #verde      @ r1 -> coloca verde do farol
    mov r2, #amarelo    @ r2 -> coloca verde do farol
    mov r3, #vermelho   @ r3 -> coloca verde do farol
    mov r4, #2          @ r4 -> controle e verificacao do estado atual
    mov r5, #0xa0000    @ r5 -> endereco da saida do botao

estado1:
    strb r1, [r0, #-1]! @ coloca verde na rua 1
    strb r3, [r0, #1]!  @ coloca vermelho na rua 2
    b loop

estado2:
    strb r2, [r0, #-1]! @ coloca amarelo nas duas ruas
    strb r2, [r0, #1]!
    b loop

estado3:
    strb r3, [r0, #-1]! @ coloca vermelho na rua 1
    strb r1, [r0, #1]!  @ coloca verde na rua 2

loop:
    ldrb r6, [r5]       @ pega a saida do teclado
    cmp r6, #1          @ se r6 == 1, estado deve mudar
    beq muda_estado
    b loop

@ r4 indica para qual estado o farol deve mudar
@ 2 e 4 indicam o estado2, e depois de 4 r4 vai para 1
@ de forma que o ciclo fica 1 -> 2 -> 3 -> 2 -> 1 -> 2 -> 3 -> ...
muda_estado:
    cmp r4, #1
    moveq r4, #2
    beq estado1
    cmp r4, #2
    moveq r4, #3
    beq estado2
    cmp r4, #3
    moveq r4, #4
    beq estado3
    cmp r4, #4
    moveq r4, #1
    beq estado2
