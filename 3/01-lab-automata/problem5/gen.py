from random import *

alpha = "abcdefghijklmnopqrstuvwxyz"

n = 100
m = 100
l = randint(1, 3 * n)
pt = 0.3

terminals = [True if random() < pt else False for i in range(n)]
k = sum(map(lambda x : 1 if x else 0, terminals))
print (n, m, k, l)
print (" ".join(map(str, [(i + 1) for i in range(n) if terminals[i]])))

s = set()
for i in range(m):
    a, b, c = randint(1, n), randint(1, n), choice(alpha)
    print (a, b, c)

