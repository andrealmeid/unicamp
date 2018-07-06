/* NOTA IMPORTANTE: em prolog, so eh possivel acessar um elemento singular de uma
   lista se esse elemento for a cabeca da lista. Se o elemento for a cauda, ele sera
   toda a cauda */

/* verifica se X eh a cabeça da lista Y */
cabeca(X, Y) :- Y = [X|_].

/* verifica RECURSIVAMENTE se X eh a cabeca da lista Y
   ou seja, verifica se X esta contido na lista Y */
/* a funcao recursiva declara Z como a cauda de Y, de modo que o tamanho
   de Z sera Y-1, Z-1, Z-1-1,... ate que a condicao se torne verdadeira
   ou que Z seja uma cauda vazia */
contido(X, Y) :- Y = [X|_].
contido(X, Y) :- Y = [_|Z], contido(X, Z).

/* versao simplificada da recursao da funcao contido */
contido_s(X, [X|_]).
contido_s(X, [_|Y]) :- contido_s(X, Y).

/* verifica se X eh a cauda de Y */
caudas_iguais(X, Y) :- Y = [_|X].

/* verifica se exite cauda */
existe_cauda(Y) :- Y \= [_].

/* encontra o ultimo elemento da cauda Y e guarda na variavel N se
   ela nao for instanciada ou compara com N se ela for instanciada */
ultimo_elemento(Y, N) :- Y = [X], N = X.
ultimo_elemento(Y, N) :- Y = [_|Z], ultimo_elemento(Z, N).

/* remove um elemento X de uma lista L */

d(X,[X|Tail],Tail).
d(X,[Y|Tail],[Y|Tail1]):-
	write('L1'), write([Y|Tail]), nl,
	write('L2'), write([Y|Tail1]), nl,
	d(X,Tail,Tail1).

/* Exercícios */
/* 1.  Escreva um predicado last(L, X) que é satisfeito
quando o termo X é o último elemento da lista L. */
last(L, X) :- L = [N], X = N.
last(L, X) :- L = [_|L2], last(L2, X).

/* 2.  Escreva um predicado efface(L1, X, L2) que é satisfeito quando
L2 é a lista obtida pela remoção da primeira ocorrência de X em L1.*/


/* 3.  Escreva um predicado delete(L1, X, L2) que é satisfeito quando
L2 é a lista obtida pela remoção de todas as ocorrências de X em L1.*/
