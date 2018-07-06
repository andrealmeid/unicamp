# Exercicio 1 - MS 211 - Turma L
# Zero de funcao - Bisseccao; Newton; Secante
# Andre Figueiredo de Almeida
# RA: 164047

import math

# n = numero maximo de iteracoes
n = 100

def calcularFuncao(x):
    ex = math.exp(x) #ex = e^x
    f = 1000000 * ex + 50000/x * (ex - 1) - 1640470
    return f

def calcularDerivadaFuncao(x):
    ex = math.exp(x)
    f = ( 50000 * (ex * (20* x*x + x - 1) + 1) ) / (x*x)
    return f

def bisseccao(a, b, p):
    fa = calcularFuncao(a)
    for i in range(0, n):
        y = (a + b)/2
        fy = calcularFuncao(y)
        if math.fabs(fy) < 10**-p:
            return y
        if fy * fa > 0:
            a = y
            fa = fy
        else:
            b = y

def secante(x0, x1, p):
    for i in range(0, n):
        x2 = (x0*calcularFuncao(x1) - x1*calcularFuncao(x0)) / (calcularFuncao(x1) - calcularFuncao(x0))
        #d = diferenca entre as p casas decimais de x(k+1) e x(k)
        d = math.fabs(int(x2 * 10**p) - int(x1 * 10**p))
        if d == 0:
            return x2
        x0 = x1
        x1 = x2

def newton(x0, p):
    for i in range(0, n):
        x1 = x0 - calcularFuncao(x0)/calcularDerivadaFuncao(x0)
        d = math.fabs(int(x1 * 10**p) - int(x0 * 10**p))
        if d == 0:
            return x1
        x0 = x1

#p = precisao desejada
p = 6

print("- Bisseccao - ")
print("Calculando f(a) = f(0.1) = " + str(calcularFuncao(0.1)))
print("Calculando f(b) = f(2)   = " + str(calcularFuncao(2)))
print("f(a)*f(b) = " + str(calcularFuncao(0.1) * calcularFuncao(2)))
print("f(a)*f(b) < 0 == " + str(calcularFuncao(0.1) * calcularFuncao(2) < 0))
print("Fazendo bisseccao em [0.1, 2] com 1 de precisao:")
print(bisseccao(0.1, 2, 1))

print("\n- Newton - ")
print("Fazendo por metodo de Newton com " + str(p) + " digitos de precisao:")
print(newton(bisseccao(0.1, 2, 1), p))

print("\n- Secante - ")
print("Fazendo por metodo da secante com " + str(p) + " digitos de precisao:")
print(secante(bisseccao(0.1, 2, 1), bisseccao(0.1, 1.5, 1), p))


# esse programa tambem plota o grafico da funcao
# essas ultimas linhas fazem isso
# em caso de erro nessa parte, por favor desconsiderar

def imprimeGrafico(a, b):
    # imprime o grafico da funcao no intervalo a <= x <= b
    # com 0.0001 de precisao
    x_pontos = []
    y_pontos = []
    i = a
    while i <= b:
        if i != 0:
            x_pontos.append(i)
            y = calcularFuncao(i)
            y_pontos.append(y)
            #descomente a linha abaixo para ver as coordenadas
            #print('(' + str(i) + ', ' + str(y) + ')')
        i = i + 0.0001

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.axhline(color='black')
    ax.axvline(color='black')

    plt.plot(x_pontos, y_pontos, linewidth=3)
    plt.xlabel("Anos")
    plt.ylabel("Populacao")
    plt.title("Crescimento populacional")
    plt.show()

import matplotlib.pyplot as plt
print("\nPlotando grafico...")
imprimeGrafico(-5, 5)
