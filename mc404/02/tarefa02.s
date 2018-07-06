@ tarefa02 - Soma de vetores
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

        .org 0x132

init:
        set r0, 0x0         @ resultado
        ldb r1, 0x100       @ divisor
        ldb r2, 0x104       @ tamanho do vetor
        set r3, 0x108       @ ponteiro de inicio do vetor
        set r4, 0x0000ffff  @ bitmask dos bits da direita
        jmp set2

set2:                       @ esses tres rotulos comparam o valor do divisor
        cmp r1, 0x4         @ e alteram ele conforme lg (r1)
        jz  set4
        cmp r1, 0x8
        jz set8
        set r1, 0x1         @ 0x2 -> 0x1
        jmp loop

set4:
        set r1, 0x2         @ 0x4 -> 0x2
        jmp loop

set8:
        set r1, 0x3         @ 0x8 -> 0x3
        jmp loop

loop:
        cmp r2, 0x0         @ verifica se acabou o vetor
        jle end
        ld  r5, [r3]        @ pega a palavra inteira
        mov r6, r5          @ copia a palavra
        and r6, r4          @ pega a primeira metade
        shr r6, r1          @ deslocamento de bits conforme divisor
        add r0, r6          @ add no resultado
        sub r2, 0x1
        cmp r2, 0x0         @ verifica se acabou o vetor
        jle end
        mov r6, r5          @ pega a palavra de novo
        shr r6, 0x10        @ desloca ate a segunda metade
        shr r6, r1          @ deslocamento de bits conforme divisor
        add r0, r6          @ add no resultado
        sub r2, 0x1
        add r3, 0x4         @ avanca pra proxima palavra
        jmp loop

end:
        hlt
