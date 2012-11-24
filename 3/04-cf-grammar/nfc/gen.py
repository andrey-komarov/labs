from random import *

nterm = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[:4]
term = "abcdefghijklmnopqrstuvwxyz"[:4]

n = 100
m = 100

print (n, choice(nterm))
for i in range(n):
    if randint(1, 2) == 1:
        print (choice(nterm), "->", choice(term))
    else:
        print (choice(nterm), "->", choice(nterm) + choice(nterm))
print ("".join(choice(term) for i in range(m)))
