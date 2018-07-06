import Data.List (transpose)
-- quicksort
quicksort :: Ord a => [a] -> [a]
quicksort [] = []
quicksort (x:xs) = (quicksort (small xs)) ++ [x] ++ (quicksort (big xs))
    where small xs = [ y | y <- xs, y <= x]
          big xs = [ y | y <- xs, y > x]

-- mergesort
merge :: Ord a => [a] -> [a] -> [a]
merge [] l = l
merge l [] = l
merge (l1:l11) (l2:l22) =
    if l1 < l2 then (l1:(merge l11 (l2:l22)))
    else (l2:(merge l22 (l1:l11)))

mergesort :: Ord a => [a] -> [a]
mergesort [] = []
mergesort [x] = [x]
mergesort l =  merge (mergesort (take half l)) (mergesort (drop half l))
    where half = div (length l) 2

-- soma de matrizes
somatrix :: Num a => [[a]] -> [[a]] -> [[a]]
somatrix [] [] = []
somatrix (l1:l11) (l2:l22) = (zipWith (+) l1 l2): somatrix l11 l22

-- multiplicacao de matrizes
multitrix :: Num a => [[a]] ->  [[a]] ->  [[a]]
multitrix [] [] = []
multitrix (l1:l11) l2 =
    [[foldl (+) 0 (zipWith (*) l1 (map head l2)) , foldl (+) 0 (zipWith (*) l1 (map head (map tail l2)))],
    [foldl (+) 0 (zipWith (*) (head l11) (map head l2)), foldl (+) 0 (zipWith (*) (head l11) (map head (map tail l2)))]]
    --[[foldl (+) 0 (zipWith (*) l1 (map head l2)), foldl (+) 0 (zipWith (*) l1 (map head (map tail l2)))],
    --[foldl (+) 0 (zipWith (*) (head l11) (map head l2)), foldl (+) 0 (zipWith (*) (head l11) (map head (map tail l2)))]]
    -- ++ zipWith (*) (head l11) (map head (map (drop 1) l2))

multmat [] _ = []
multmat m1 m2 = mm m1 (transp m2)

prodint l1 l2 = foldl (+) 0 (zipWith (*) l1 l2)

mm m1 m2 = map (\ l1 -> map (\ l2 -> prodint l1 l2) m2 ) m1

transp ([]:_) = []
transp m = map head m : transp (map tail m)

mmult :: Num a => [[a]] -> [[a]] -> [[a]]
mmult a b = [[ sum $ zipWith (*) ar bc | bc <- (transpose b)] | ar <- a ]
