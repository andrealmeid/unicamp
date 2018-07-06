rest(L, R) :- L = [_|R].
first(L, F) :- L = [F|_].

%% tamanho de uma lista

tamanho([], 0).
tamanho([_|T], L) :- tamanho(T, N), L is N+1.


%% somar todos os elementos de uma lista

somar([], 0).
somar([H|T], S) :- somar(T, N), S is N+H.


%% achar n-esimo elemento da lista

nesimo(0, [X|_], X).
nesimo(N, [_|T], E) :- M is N-1, nesimo(M, T, E).

%% achar o ultimo elemento da lista

ultimo([X], X).
ultimo([_|R], X) :- ultimo(R, X).

%% achar o primeiro indice de um item numa lista

findfirst(-1, [], _).
findfirst(0, [X|_], X).
findfirst(I, [_|T], X) :- findfirst(J, T, X), I is J+1, J > -1.

%%  numero de vezes que um elemento aparece numa lista

qntd([], _, 0).
qntd([F|R], F, N) :- qntd(R, F, M), N is M+1.
qntd([_|R], E, N) :- qntd(R, E, N).


%% inverter uma lista

inverter(L, R) :- inverterMesmo(L, [], R).
inverterMesmo([], P, P).
inverterMesmo([F|R], P, X) :- inverterMesmo(R, [F|P], X).


%% achar o maior elemento da lista
maiorMesmo([], M, M).
maiorMesmo([F|R], M, V) :- F > M, maiorMesmo(R, F, V).
maiorMesmo([_|R], M, V) :- maiorMesmo(R, M, V).
maior([F|R], V) :- maiorMesmo(R, F, V).
