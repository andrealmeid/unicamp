@ tarefa01 - Invertendo uma cadeia de caracteres
@ Turma B
@ Andre Figueiredo de Almeida
@ RA 164047

start:
        set r1, 0x1000  @ registrador que ira apontar para o final

getEnd:                 @ encontra o final da sequencia
        ldb r0, [r1]
        cmp r0, 0x0     @ verifica se o byte atual == 0x0
        jz  setEnd
        
        add r1, 0x1
        jmp getEnd

setEnd:
        set r0, 0x1000  @ registrador para o inicio da sequencia
        sub r1, 0x1     @ corrige o valor do final da sequencia

revert:
        mov r4, r1      @ pega o ponteiro final
        sub r4, r0      @ e verifica se nao ultrapassou o inicial
        cmp r4, 0x0
        jle end

        ldb r2, [r0]    @ pega o inicio da sequencia
        ldb r3, [r1]    @ pega o fim da sequencia
        stb [r0], r3    @ faz a troca
        stb [r1], r2
        add r0, 0x1     @ avanca com o registrador do inicio
        sub r1, 0x1     @ retrocede com o registrador do final
        jmp revert

end:
        hlt
