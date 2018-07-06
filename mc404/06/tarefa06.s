@ tarefa06 - Tudo em Maiusculas
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

STDIN   .equ 0          @ descritor stdout
STDOUT  .equ 1          @ descritor stdout
READ    .equ 3          @ tipo read
WRITE   .equ 4          @ tipo write
EXIT    .equ 1          @ tipo exit
ARRAY   .equ 0x2000     @ endereco inicial do vetor
MAX     .equ 256        @ numero maximo de caracteres

    .org 0x1000         @ reserva de espaco para o susy

fim_pilha:
    .skip  0x500
ini_pilha:

init:
    set  sp, ini_pilha  @ inicia a pilha
    call le_cadeia      @ rotina de leitura da frase
    call maiuscula_init @ rotina para deixar caracteres maiusculos
    call escreve_cadeia @ rotina para imprimir frase final
    call end            @ rotina para finalizar a execucao

le_cadeia:
    set r0, STDIN       @ define r0, r1, r2 e r7 conforme as especificacoes
    set r1, ARRAY       @ do tty para entrada de dados
    set r2, MAX
    set r7, READ
    sys 0x55            @ chama a interrupcao e retorna
    ret

maiuscula_init:
    set r10, 0          @ zera o contador de minuscula
    set r0, ARRAY       @ aponta para o inicio da frase

maiuscula_loop:
    ldb r1, [r0]        @ pega um elemento do vetor
    cmp r1, 0           @ verifica se eh o ultimo
    jz  maiuscula_end   @ se sim, finaliza rotina maiuscula
    cmp r1, 0x60        @ se r2 > 0x60, talvez seja minuscula
    jg  maiuscula_ver   @ rotina para verificar se eh minuscula
    add r0, 1           @ vai para proximo caractere
    jmp maiuscula_loop

maiuscula_ver:
    cmp r1, 0x7b        @ se r2 > 0x60 e r2 < 0x7B, eh uma letra minuscula
    jl  maiuscula_add   @ rotina para tornar minuscula e incrementar r10
    add r0, 1
    jmp maiuscula_loop

maiuscula_add:
    sub r1, 0x20        @ deixa o caractere maiusculo
    stb [r0], r1        @ salva na memoria
    add r10, 1          @ add 1 no contador de minusculas
    add r0, 1           @ vai para proximo caractere
    jmp maiuscula_loop

maiuscula_end:
    ret

escreve_cadeia:
    set r0, STDOUT      @ define r0, r1, r2 e r7 conforme as especificacoes
    set r1, ARRAY       @ do tty para entrada de dados
    set r7, WRITE
    set r2, ARRAY       @ r2 aponta para inicio da cadeia
    sub r2, 1

escreve_loop:
    add r2, 1           @ anda uma posicao
    ldb r3, [r2]        @ carrega caractere
    cmp r3, 0           @ verifica se eh o fim da cadeia
    jnz escreve_loop    @ se nao, continua na cadeia
    sub r2, r1          @ r2 - r1 =  numero de caracteres da cadeia
    sys 0x55            @ executa chamada ao sistema
    ret

end:
    set r0, 0           @ define r0 como 0 para indicar que nao ouve erro
    set r7, EXIT        @ define r7 como constante de saida do programa
    sys 0x55            @ chama interrupcao
    ret
