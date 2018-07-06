pai(betao, tony).
pai(betao, marcelo).

pai(tony, tonynho).
pai(tony, tonyJR).
pai(tony, julianaJR).

pai(marcelo, claudio).
pai(marcelo, claudia).

masc(betao).
masc(tony).
masc(marcelo).
masc(tonynho).
masc(tonyJR).

fem(claudio).
fem(claudia).
fem(julianaJR).
fem(marcelo).

/* criar vo, mae, sobrinho e primo */

irmao(X, Y) :- pai(Z, X), pai(Z, Y), \+ X = Y.

vo(X, Y) :- pai(X, Z), pai(Z, Y).

sobrinho(X, Y) :- pai(Z, X), irmao(Z, Y).

primo(X, Y) :- pai(Z, X), pai(W, Y), irmao(Z, W).

igual(X, X).

descendente(X, Y) :- pai(X, Y).
descendente(X, Y) :- pai(X, Z), descendente(Z, Y).

herdeiro(X, Y) :- descendente(X, Y), masc(Y).

errogene1() :- pai(X, Y), fem(X).

arco(a, b).
arco(b, c).
arco(c, d).
arco(c, a).

caminho(X, Y) :- arco(X, Y).
caminho(X, Y) :- arco(X, Z), caminho(Z, Y).

idaevolta(X, Y) :- caminho(X, Y), caminho(Y, X).
