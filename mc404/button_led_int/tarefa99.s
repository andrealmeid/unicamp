@ Programa para ligar/desligar um led, atuando um botão

        .global _start         @ ligador precisa do símbolo _start

@ flag para habilitar/desabilitar interrupções externas no registrador de status
        .equ IRQ, 0x80
        .equ FIQ, 0x40

@ modos de execução no registrador de status
        .equ FIQ_MODE,0x11
        .equ IRQ_MODE,0x12
        .equ USER_MODE,0x10


@ vetor de interrupções
        .org   7*4               @ preenche apenas a posição 7 (FIQ)
        b      tratador_botao

        .org 0x800
_start:
        mov     sp,#0x400       @ seta pilha do modo supervisor
        mov     r0,#FIQ_MODE    @ coloca processador no modo FIQ (interrupção externa)
        msr     cpsr,r0         @ esta é uma instrução especial, processador agora no modo FIQ
        mov     sp,#0x300       @ seta pilha de interrupção do modo FIQ -- nem precisa se o tratador não for
	                        @ usar a pilha, apenas para mostrar como pode ser feito
        mov     r0,#USER_MODE   @ coloca processador no modo usuário
        orr     r0,r0,#(IRQ)    @ com interrupções IRQ desabilitadas (bit F==1)
        bic     r0,r0,#(FIQ)    @ com interrupções FIQ habilitadas (bit I==1) -- nem precisa desta instrução,
	                        @ pois o bit F já é zero aqui. Apenas para mostrar como poderia desligar o bit F
        msr     cpsr,r0         @ esta é uma instrução especial, processador agora no modo usuário
        mov     sp,#0x80000     @ seta pilha do usuário no final da memória

loop:
        b       loop

@ tratador da interrupcao
@ vamos usar apenas os registradores r8 e r9, para não precisar usar a pilha para salvar
tratador_botao:
        b exit


exit:
    mov r0, #0
    mov r7, #1
    swi #0x55
