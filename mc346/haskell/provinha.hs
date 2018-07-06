merge :: Ord a => [a] -> [a] -> [a]
merge [] l = l
merge l [] = l
merge (l1:l1s) (l2:l2s) =
    if l1 < l2 then (l1:(merge l1s (l2:l2s)))
    else (l2:(merge l2s (l1:l1s)))

mergesort :: Ord a => [a] -> [a]
mergesort [] = []
mergesort [x] = [x]
mergesort l =  merge (mergesort (take half l)) (mergesort (drop half l))
    where half = div (length l) 2
