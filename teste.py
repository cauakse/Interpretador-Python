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
print("Meu nome e %s" % nome)
if c>1:
   print(fatorial(b))
