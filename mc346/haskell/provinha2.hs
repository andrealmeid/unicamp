decode :: Num a => [(b, a)] -> [b]
decode [] = []
decode ((ch, num):xs)
    | num == 1 = [ch]
    | num == 0 = decode xs
    | otherwise = ch : decode [(ch, num-1)]
