soma1([], X, [[X, 1]]).
soma1([[Ch,Q]|R], Ch, Novo) :-  Qq is Q+1, Par = [Ch, Qq], Novo = [Par|R].
soma1([F|R], Ch, Novo) :- Novo = [F|Novo2], soma1(R, Ch, Novo2).

contapal(L, Dic) :- contapal(L, [], Dic).
contapal([], Dic, Dic).
contapal([Pal|Resto], Dic, NovoDic2) :- soma1(Dic, Pal, NovoDic), contapal(Resto, NovoDic, NovoDic2).
