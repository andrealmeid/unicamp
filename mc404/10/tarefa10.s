@ tarefa10 - Semaforos com temporizador
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    .global _start

@ vetor de interrupções
    .org   7*4          @ preenche apenas a posição 7 (FIQ
    b      tratador_temp

    .org 500
_start:
    @ faz tratamento de habilitacao de interrupcao
    mov r0, #0x11       @ coloca processador no modo FIQ
    msr cpsr, r0        @ processador agora no modo FIQ
    orr r0, r0, #0x80   @ com interrupções IRQ desabilitadas (bit F==1)
    msr cpsr, r0        @ processador agora no modo usuário

    mov r9, #0x90000    @ registrador que guarda 0x90000
    mov r10, #2         @ amarelo
    mov r8, #4          @ vermelho
    mov r5, #1          @ verde

    @ seta temporizador para 1 seg
    mov r0, #1000        @ 1000ms == 1s
    add r1, r9, #0x30    @ 0x90030 == endereco do temporizador
    str r0, [r1]

    @ seta contador 1 com 00
    mov r0, #0x7E       @ 0x7E == 0 no display de 7 segmentos
    add r1, r9, #0x20   @ 0x90020 == digito menos significativo
    strb r0, [r1]
    add r1, #1          @ 0x90021 == digito menos significativo
    strb r0, [r1]

    @ seta contador 0 com 30
    sub r1, #0x10       @ 0x90011 == digito menos significativo
    strb r0, [r1]
    mov r6, #3          @ r6 -> digito mais significativo
    mov r7, #0          @ r7 -> digito menos significativo
    mov r0, #digitos    @ vetor de digitos do display de segmentos
    add r0, r6          @ digitos[3] == 3 no display de 7 segmentos
    ldrb r2, [r0]
    sub r1, #0x1        @ 0x90010 == digito mais significativo
    strb r2, [r1]

    @seta semaforo 0
    sub r1, #0x10       @ 0x90000 == semaforo 0
    strb r5, [r1]       @ r5 == verde

    @seta semaforo 1
    strb r8, [r1, #1]   @ 0x90001 == semaforo 1; r8 == vermelho

    @ r4 -> indica semaforo atual: 0x11 == semaforo 0; 0x21 semaforo 1
    mov r4, #0x11
    mov r2, #0

main_loop:
    cmp r2, #0          @ se r2 == 0, não houve interrupcao
    beq main_loop

muda_menos:
    cmp r7, #-1         @ se digito menos significativo == -1,
    beq muda_mais       @ sub 1 do mais significativo

    mov r0, #digitos    @ r0 = *digitos[0]
    ldrb r3, [r0, r7]   @ r3 = *digitos[r7]
    add r2, r4          @ 0x90000 + r4; (r4 = 0x11 | 0x21)
    strb r3, [r2]       @ semaforo[r2] = digitos[r7]
    mov r2, #0          @ desliga flag de interrupcao

    add r0, r6, r7      @ se a soma dos dois digitos == 0, acabou o tempo
    cmp r0, #0
    beq acabou_tempo

     cmp r7, #5          @ se digito menos significativo != 5, continua
    bne main_loop
    cmp r6, #0          @ se contador == 05, deixar set_amarelo
    bne main_loop

set_amarelo:
    strb r10, [r9, #1]!  @ semaforo 0 (0x90000) = amarelo == r10
    strb r10, [r9, #-1]! @ semaforo 1 (0x90001) = amarelo == r10
    b main_loop

muda_mais:
    sub r6, #1          @ mais significativo -= 1
    add r0, r6, #digitos@ r0 = digitos[r6]
    sub r4, #1          @ 0x11 = 0x10 | 0x21 = 0x20
    add r2, r4          @ r2 = 0x90010 | 0x90020
    add r4, #1          @ reseta r4
    ldrb r3, [r0]       @ r3 = digitos[r0]
    strb r3, [r2]       @ [r2] = r3
    mov r7, #9          @ menos significativo = 9
    mov r0, #0x7B       @ 0x7B == 9 no display de 7 segmentos
    add r2, #1          @ r2 = 0x90011 | 0x90021
    strb r0, [r2]       @ [r2] = 9 no display de 7 segmentos
    mov r2, #0          @ desliga flag de interrupcao
    b main_loop

tratador_temp:
    mov r2, r9          @ r2 = 0x90000
    sub r7, #1          @ digito menos significativo -= 1
    movs pc,lr

acabou_tempo:
    mov r3, r9          @ r3 = 0x90000
    mov r6, #3          @ reinicia digitos, contador = 30
    mov r7, #0
    cmp r4, #0x11       @ se r4 == 0x11, farol atual == 0, logo
    beq set_farol1      @ devemos preparar farol 1 e contador 1

set_farol0:
    strb r8, [r3, #1]   @ coloca vermelho no farol 1
    strb r5, [r3]       @ verde no farol 0
    mov r4, #0x11       @ seta farol atual com 0
    b set_farol

set_farol1:
    strb r5, [r3, #1]   @ coloca verde no farol 1
    strb r8, [r3]       @ vermelho no farol 0
    mov r4, #0x21       @ seta farol atual com 1

set_farol:
    add r0, r6, #digitos@ r0 = digitos[r6]
    ldrb r3, [r0]       @ r3 = [r0]
    sub r4, #1          @ r4 = 0x10 | 0x20
    add r2, r9, r4      @ r2 = 0x90010 | 0x90020
    strb r3, [r2]       @ [r2] = r3
    add r4, #1          @ r4 = 0x11 | 0x21
    mov r2, #0          @ desliga flag de interrupcao
    b muda_menos

digitos:
    .byte  0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b
