def soma (x,y):
   return x + y
def fatorial (x):
   fatorial = 1
   for i in range(1,x+1):
      fatorial *= i
   return fatorial
a = 2*2
b=5
c = 4*(2+2)
nome = 'Matheus Oliveira da Silva'
d = 2
print("%d" % b)
print("a = %d e c = %f" % (a,c))
if b < 5:
   print("b < 5")
else:
   print("b >= 5")
print('Meu nome e',nome)
print("Meu nome e",nome,a,b,c)
if b >= 5:
   print("Meu nome e" + nome)
else:
   print("Meu nome nao e" + nome)
print("Fim do programa")
