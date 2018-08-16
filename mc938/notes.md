# MC938 - Algoritmos Criptográfico

## 1. Introdução

### História

Até anos 70, criptografia focada em militar/diplomacia. Turing desenvolve
máquina para quebrar criptografia da máquina dos nazistas. Em 1976, começam
 aplicações para comunicação em redes e uso comercial da criptografia.

Anos 80: consilidação do `RSA`.

Anos 90: ECC vem aumentando, DES morre e da lugar pro AES. Algoritmos 
quânticos para cripto.

Anos 2000: protocolos criptográficos mais formais, em busca do _hash_ perfeito,
algoritmos pós-quanticos.

Atualmente, o nome criptografia é usado para coisas além de funções de cifrar
e decifrar, como blockchain, hash, criptomoedas, etc.

### Modelo de comunicação

Alice conversando com Beto por canal inseguro, onde Mauro quer interferir/escutar
.

### Criptografia simétrica

Mesma chave para cifrar/decifrar e o algoritmo é público. Para ser seguro, precisamos que:

- Enc.(.) deve ser difícil de ser decifrado com força bruta, e ser uma função de mão-única. O número de possíveis chaves _k_ deve ser muito grande.
