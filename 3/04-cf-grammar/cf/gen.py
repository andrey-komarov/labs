from random import *

nterm = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[:]
term = "abcdefghijklmnopqrstuvwxyz"[:]

n = 4
m = 5
T = 1

print (n, choice(nterm))
for i in range(n):
    t = randint(0, T)
    print (choice(nterm), "->", "".join(choice(nterm+term) for i in range(t)))
print ("".join(choice(term) for i in range(m)))
