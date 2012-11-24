from random import *

alpha = "abcdefghijklmnopqrstuvwxyz"

n = 100000
m = 100000
pt = 0.1

terminals = [True if random() < pt else False for i in range(n)]
k = sum(map(lambda x : 1 if x else 0, terminals))
print (n, m, k)
print (" ".join(map(str, [(i + 1) for i in range(n) if terminals[i]])))

s = set()
for i in range(m):
    a, b, c = randint(1, n), randint(1, n), choice(alpha)
    while (a, c) in s:
        a, b, c = randint(1, n), randint(1, n), choice(alpha)
    s.add((a, c))
    print (a, b, c)

