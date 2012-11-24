from random import *

n = 15
alpha = "abcdefghijklmnopqrstuvwxyz"[:3]
p = 0.4
p2 = 0.3

e = []
for i in range(n):
    for ch in alpha:
        if random() < p:
            e.append((i+1, randint(1, n), ch))

t = [i for i in range(1, n + 1) if random() < p2]
while len(t) == 0:
    t = [i for i in range(1, n + 1) if random() < p2]
print (n, len(e), len(t))
print (" ".join(map(str, t)))
for (f, t, ch) in e:
    print (f, t, ch)

