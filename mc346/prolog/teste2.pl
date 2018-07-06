% cadak([], _, [], _).
% cadak([H], 1, [H], _).
% cadak([H], _, [], _).
% cadak([_|T], K, L, O) :- Kk is K-1, cadak([T], Kk, L, O).
% cadak([H|T], 1, L, O) :- Ll is [H|L], cadak([T], O, Ll, O).

cadak([], _, []).
cadak(L, K, O) :- (length(L, S), S >= K ->
    Ke is K-1, nth0(Ke, L, X), O = [X|O2], right(L, K, Rlist),
    cadak(Rlist, K, O2); O = []).
