@ tarefa00 - Familiarizacao com as ferramentas
@ Andre Figueiredo de Almeida
@ Turma B
@ RA 164047

    .org  0x200

start:
    set   r0, 0x5000  @ coloca 0x5000 no registrador r0
    set   r1, 0x200   @ coloca 0x200 no registrador r1
    add   r0, r1      @ adiciona r1 no r0, entao r0 = r1 + r0 = 0x5200
    add   r0, r0      @ faz 2 * r0
    add   r0, r0      @ faz 2 * (2 * r0) = 4 * r0 = 4 * (r0 + r1)
    hlt
