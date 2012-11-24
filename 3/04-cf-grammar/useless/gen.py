from random import *

nterm = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[:]
term = "abcdefghijklmnopqrstuvwxyz"[:]

n = 100
m = 3

print (n, choice(nterm))
for i in range(n):
    if randint(1, 2) == 1:
        print (choice(nterm), "->", choice(term))
    else:
        print (choice(nterm), "->", choice(nterm) + choice(nterm))
#print ("".join(choice(term) for i in range(m)))
