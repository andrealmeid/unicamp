@ tarefa03 - Soma de vetore de 64 bits
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

@ posiciona o vetor e num_elem na memoria
        .org 0x100
num_elem:
        .skip 4
vetor:
        .skip 920

@ posiciona a pilha na memoria
        .org 0x4000
fim_pilha:
        .skip  500
ini_pilha:

@ menos = menos signficativo
@ mais = mais signficativo

        .org 0x1000
add64:
        ld r1, [sp+16]      @ pega o menos da soma atual
        ld r0, [sp+12]      @ pega o mais da soma atual
        ld r3, [sp+8]       @ pega o menos do numero atual
        ld r2, [sp+4]       @ pega o mais do numero atual

        add r0, r2          @ adiciona o menos
        jc carry            @ trata o carry
        add r1, r3          @ adiciona o mais
        ret

carry:
        add r1, 0x1         @ adiciona o carry
        add r1, r3          @ adiciona o mais
        ret

        .org 0x2000
init:
        set sp, ini_pilha   @ inicia a pilha
        set r0, 0x0         @ inicia onde vai ficar o numero final
        set r1, 0x0
        set r8, vetor       @ pega a cabeca do vetor
        ld  r9, num_elem    @ pega o num de elem do vetor
        jmp loop

loop:
        set sp, ini_pilha   @ reinicia a pilha
        cmp r9, 0x0         @ verifica se acabaram os elementos
        jz end              @ acaba o programa
        ld  r2, [r8]        @ pega o menos do vetor
        ld  r3, [r8+4]      @ pega o mais do vetor
        push r1             @ envia a soma total na ordem do problema
        push r0
        push r3             @ envia o numero atual na ordem do problema
        push r2
        call add64          @ chama a funcao de adicionar
        sub r9, 0x1         @ diminui um elemento de num_elem
        add r8, 0x8         @ avanca para o proximo numero
        jmp loop

end:
        hlt
