@ tarefa08 - Palindromo em bits
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

    .global _start

_start:
    @ read do console e guarda em 'msg'
    mov r0, #0
    ldr r1, =msg
    mov r2, #8
    mov r7, #3
    swi #0x55

    mov r10, #0     @ zera r10
    ldrb r2, [r1]   @ pega o primeiro char
    cmp r2, #0x30   @ verifica se == '0'
    bne converter   @ se nao, converte de ascii -> hex
    cmp r0, #1      @ se sim, verifica se o tamanho da entrada == 1
    beq exit        @ se sim, acabou o programa


@ pega o endereco da entrada em r1, converte a entrada de
@ ascii para hexadecimal e armazena esse numero em r10
converter:
    cmp r2, #0x40   @ verifica se r2 esta entre [A-F]
    subhi r2, #0x37 @ se sim, subtrai 0x37
    subls r2, #0x30 @ se nao, subtrai 0x30
    add r10, r2     @ insere o numero em r10
    cmp r0, #1      @ verifica se esse foi o menos significativo
    beq set_palindro
    lsl r10, #4     @ se nao, da shift
    sub r0, #1      @ diminui contador de bits
    add r1, #1      @ avanca na palavra
    ldrb r2, [r1]   @ pega proximo caracter
    b converter

@ reseta as mascaras
set_palindro:
    mov r1, #1
    mov r2, #0x80000000

@ percorre os bits da palavra, pegando um da esquerda e o seu simetrico
@ da direta. aplica mascara, verifica se eh 0 ou 1 e compara os dois
loop_palindro:
    cmp r1, r2      @ se r1 > r2, nao houve ocorrencia de diferenca
    ldrhi r1, =s    @ portando, eh um palindromo
    bhi write
    and r3, r1, r10 @ aplicacao de mascaras
    and r4, r2, r10
    cmp r3, #0      @ se r3 > 0, r3 = 1
    movhi r3, #1
    cmp r4, #0      @ se r4 > 0, r4 = 1
    movhi r4, #1
    cmp r4, r3      @ se r3 != r4,
    ldrne r1, =n    @ nao eh palindromo
    bne write
    lsl r1, #1      @ avanca com as mascaras para dentro da palavra
    lsr r2, #1
    b loop_palindro

write:
    mov r0, #1
    mov r2, #2
    mov r7, #4
    swi #0x55
    b _start

exit:
    mov r0, #0
    mov r7, #1
    swi #0x55

s:
    .ascii "S\n"
n:
    .ascii "N\n"

msg:
    .skip 8
