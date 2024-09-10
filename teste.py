def soma (x,y):
   return x + y
def fatorial (x):
   fatorial = 1
   for i in range(1,x+1):
      fatorial *= i
   return fatorial

a = 2
b = 3
c = soma(a,b)
print(c)
if c>1:
   print(fatorial(c))
