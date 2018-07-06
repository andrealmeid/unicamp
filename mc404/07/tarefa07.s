@ tarefa07 - Familiarizacao com as ferramentas ARM
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    .global _start
    .org 0x1000

_start:
    @ write
    mov     r0, #1     @ fd = stdout
    ldr     r1, =msg   @ buf = msg
    ldr     r2, =len   @ numero de elementos = tamanho da mensagem
    mov     r7, #4     @ syscall #4 = write
    swi     #0x55      @ chama syscall

    @ exit
    mov     r0, #0     @ status = 0
    mov     r7, #1     @ syscall #1 = exit
    swi     #0x55      @ chama syscall

msg:
    .ascii      "Um teste simples\n"
len = . - msg
