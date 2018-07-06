-- tamanho de uma lista
tamanho list = if list == [] then 0
               else 1 + tamanho (tail list)

tamanho2 [] = 0
tamanho2 list = 1 + tamanho2 (tail list)

-- soma de todos os elementos de uma lista
somar list = if list == [] then 0
             else head list + somar (tail list)

-- achar n-esimo elemento da lista
nesimo n lista = if n == 1 then head lista
                 else nesimo (n-1) (tail lista)

nesimo2 n (h:t)
    | n == 1    = h
    | otherwise = nesimo2 (n-1) t

nesimo3 1 (h:_) = h
nesimo3 n (_:t) = nesimo3 (n-1) t

-- achar ultimo da lista
ultimo (h:t)
    | t == []   = h
    | otherwise = ultimo t

ultimo2 lista = last lista

-- buscar itemcxz
busca item (h:t)
    | h == item  = 1
    | otherwise  = 1 + busca item t

-- inverter lista
inverter [] = []
inverter (h:t) = inverter t ++ [h]

-- soma1
soma1 ch [] = [(ch,1)]
soma1 ch ((k,v):dic) = if ch == k then ((k,v+1):dic)
                       else (k,v) : soma1 ch dic


----
k_elements k list = k_elements_aux k 1 list
    where k_elements_aux 1 n list = list
          k_elements_aux k n [] = []
          k_elements_aux k n (x : xs)
               | n == k = [x] ++ k_elements_aux k (1) xs
               | otherwise = k_elements_aux k (n + 1) xs
