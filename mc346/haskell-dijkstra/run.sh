#!/bin/bash
rm -f prog2
ghc prog2.hs
if [ -f prog2 ]; then
    ./prog2
fi
#./prog2 < in/arq1.in
#./prog2 < in/arq2.in
#./prog2 < in/arq3.in
