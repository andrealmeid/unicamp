import Data.Map

list = [
    ('a', 'b', 1),
    ('b', 'f', 1),
    ('a', 'c', 2),
    ('c', 'd', 2),
    ('d', 'f', 2)
  ]

start = 'a'
end = 'f'
nodes = [(start, 0)]

-- sdds commit dos crtlz

removeFromList list char = [(x, y, z) | (x, y, z) <- list, x /= char]

isFonList list = [value | (key, value) <- list, key == 'f']

dijk nodes list = dijk (Map.toList result) list'
     where (key, value) = (head nodes)
           connected = [(x,w) | (p, x, y) <- list, p == key, let z = value, let w = y + z]
           list' = removeFromList list key
           result = Map.insertWith (min) key value $ Map.fromList (tail nodes)



-- primeiro iteracao
a1 = [(start, 0)]

a2 = [(x,y) | (p, x, y) <- list, p == fst(head a1)] -- pega todo mundo conectado ao primeiro elemento

a3 = [(x,w) | (_,z) <- a1, (x,y) <- a2, let w = y + z] -- faz o calculo do custo dos caminhos

-- segunda iteracao
b1 = [head a3]

b2 = [(x,y) | (p, x, y) <- list, p == fst(head b1)]

b3 = [(x,w) | (_,z) <- b1, (x,y) <- b2, let w = y + z]

-- terceira iteracao
c1 = tail a3

c2 = [(x,y) | (p, x, y) <- list, p == fst(head c1)]

c3 = [(x,w) | (_,z) <- c1, (x,y) <- c2, let w = y + z]

-- quarta iteracao

d2 = [(x,y) | (p, x, y) <- list, p == fst(head c3)]

d3 = [(x,w) | (_,z) <- c3, (x,y) <- d2, let w = y + z]

-- TODO: ver se a chave existe, se sim, verifica se eh menor e talz
-- mexedick chave peso dic = if Data.Map.member chave dic then
--     Data.Map.
--         else Data.Map.insert chave peso dic
--
--
-- ande inicio l d = Prelude.foldr
--     (\(a, b, c) xs -> if a == inicio then
--         mexedick b c xs
--         else xs) d l
