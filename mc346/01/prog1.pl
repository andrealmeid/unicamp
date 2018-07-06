/*  Projeto 1 - Caixeiro viajante
    Andre Figueiredo de Almeida
    RA 164047 */

% pega o custo de um caminho; -1 se o caminho nao eh possivel
getCusto(_, [], 0).
getCusto(Inicio, [Final], Resul) :-
    (arco(Final, Inicio, C), ! ->
        Resul is C;
            Resul is -1).
getCusto(Inicio, [C|[R|Rr]], Resul) :-
    (arco(C, R, Cc), ! ->
        (getCusto(Inicio, [R|Rr], Custo2), Custo2 \= -1 ->
            Resul is Cc + Custo2;
                Resul is -1);
            Resul is -1).

% gera todas as permutacoes validas para um caminho
getPermuta(Caminho, Inicio, P) :-
    delete(Caminho, Inicio, Lista),
    findall([Inicio|Y], permutation(Lista, Y), P).

%  roda o setCusto para todas as permutacoes
getMenorCusto([C], Inicio, Custo, C) :-
    getCusto(Inicio, C, Custo), !.
getMenorCusto([H|T], Inicio, Custo, Caminho) :-
    getCusto(Inicio, H, Custo1), getMenorCusto(T, Inicio, Custo2, Caminho2),
    (Custo1 = -1 ->
        Custo is Custo2, Caminho = Caminho2;
            (Custo1 >= Custo2 ->
                Custo is Custo2, Caminho = Caminho2;
                    Custo is Custo1, Caminho = H)).

% coloca todos os vertices no banco de dados
assertall([]).
assertall([H|T]) :-
    asserta(H), assertall(T).

% insere o Inicio no fim da Lista
insertI(L1, I, L2) :-
    reverse(L1, L3),
    L4 = [I|L3],
    reverse(L4, L2).

main :-
    read(Vertices),
    read(Arestas),
    read(Inicio),
    assertall(Arestas),
    getPermuta(Vertices, Inicio, P),
    sort(P, POrd),
    reverse(POrd, POrdR),
    getMenorCusto(POrdR, Inicio, Custo, Caminho),
    (Custo = -1 -> write(nada), nl;
    write(Custo), nl,
    insertI(Caminho, Inicio, Caminho2),
    write(Caminho2), nl).
