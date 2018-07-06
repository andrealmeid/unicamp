l = ["123", "abc 123", "xyz zkw"]

proc n ls = unwords (map reverse (words(ls !! n)))

main = do
    l <- getLine
    ll <- getLine
    let a = (return l:ll)
    putStrLn $ proc 1 a
