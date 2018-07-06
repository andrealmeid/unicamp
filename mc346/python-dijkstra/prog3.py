# RA: 164047
# Andre Figueiredo de Almeida

# define a constante infinito, maior que qual numero inteiro
infinito = float("inf")
import sys

# classe do dicionario de nos
# dict = lista de nos
class DictNos():
    def __init__(self):
        self.dict = []

    # pega o indice da chave no dicionario, se nao existe, retorna -1
    def getNo(self, chave):
        for no in self.dict:
            if no.chave == chave:
                return no
        return -1

    # insere um nó no dict de nos, se ele ainda nao foi inserido
    def inserir(self, chave, custo=infinito, distancia=0, velocidade=0):
        if self.getNo(chave) == -1:
            self.dict.append(No(chave, custo, distancia, velocidade))

    def remover(self, chave):
        no = self.getNo(chave)
        if no != -1:
            self.dict.remove(no)

class No():
    def __init__(self, chave, custo, distancia=0, velocidade=0, anterior=None, visitado=False):
        self.chave = chave
        self.custo = custo
        self.distancia = distancia
        self.velocidade = velocidade
        self.vizinhos = DictNos()
        self.anterior = anterior
        self.visitado = visitado

    # insere vizinhos
    def inserirVizinho(self, vizinho, custo, distancia, velocidade):
        self.vizinhos.inserir(vizinho, custo, distancia, velocidade)

    # update vizinho, se velocidade == 0, deleta
    def updateVizinho(self, vizinho, velocidade):
        if velocidade == 0:
            self.vizinhos.remover(vizinho)
        else:
            vizinho = self.vizinhos.getNo(vizinho)
            vizinho.velocidade = velocidade
            vizinho.custo = vizinho.distancia/velocidade

def caminho(de, para, distancia, velocidade):
    Nos.inserir(de)
    Nos.inserir(para)
    Nos.getNo(de).inserirVizinho(para, distancia/velocidade, distancia, velocidade)

def updateCaminho(de, para, velocidade):
    Nos.getNo(de).updateVizinho(para, velocidade)

# pega o no de pivo custo do dicionario
def getMenor(Nos):
    pivo = No(None, infinito)

    for no in Nos.dict:
        if no.custo < pivo.custo and no.visitado == False:
            pivo = no
    return pivo

def printNos(nos):
    l = []
    for no in nos.dict:
        print(no.chave, no.custo, no.anterior, no.visitado)
    print()

def dijkstra(Nos, inicio, fim):
    # inicio eh o primeiro no do caminho e tem distancia 0
    inicio = Nos.getNo(inicio)
    inicio.custo = 0

    pivo = inicio

    # enquanto houver algum pivo valido
    while pivo.chave != None:
        # pivo foi visitado
        pivo.visitado = True

        # se o pivo eh o no final, retorna a distancia dele
        if pivo.chave == fim:
            return pivo.custo

        # avalia os vizinhos do pivo atual e computa suas distancias
        for vizinho in Nos.getNo(pivo.chave).vizinhos.dict:
            no = Nos.getNo(vizinho.chave)

            custo = vizinho.custo + pivo.custo
            if no.custo > custo:
                no.anterior = pivo.chave
                no.custo = custo

        # pega o proximo pivo (no com menor distancia ainda nao visitado)
        pivo = getMenor(Nos)

    return "nada"

def getCaminho(nos, fim):
    path = [fim]
    no = Nos.getNo(fim)
    while no != -1 and no.anterior != None:
        path.append(no.anterior)
        no = Nos.getNo(no.anterior)

    path.reverse()
    return str(path)

Nos = DictNos()

def main():

    velocidade_padrao = int(input())

    update = False
    inicio = False
    for line in sys.stdin:
        b = []
        b.append(0)
        b.append(0)
        b.append(0)
        b.append(velocidade_padrao)

        if line == '\n':
            update = True
            continue

        l = line.split(" ")

        if len(l) == 1:
            ll = l[0].replace('\n', '')
            if inicio:
                fim = ll
            else:
                inicio == False
                inicio = ll
            continue

        for i in range(0, len(l)):
            if i >= 2:
                b[i] = float(l[i])
            else:
                b[i] = l[i]

        if update == False:
            caminho(b[0], b[1], b[2], b[3])

        if update == True:
            updateCaminho(b[0], b[1], b[2])


    if Nos.getNo(inicio) == -1:
        print("Origem nao existe")
        return

    if Nos.getNo(fim) == -1:
        print("Destino nao existe")
        return

    custo = dijkstra(Nos, inicio, fim)

    if custo == "nada":
        print("Nao existe caminho de " + inicio + " para " + fim)
        return

    print(str(int(custo * 60)) + " minutos")
    print(getCaminho(Nos, fim))

if __name__ == "__main__":
    main()
