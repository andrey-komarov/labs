import sys
n, r = map(int, open('num2part.in').read().split())

f = [[0] * (n + 1) for i in range(n + 1)]
for i in range(n + 1):
    f[i][i] = 1
for i in range(n + 1):
    for j in range(1, i):
        f[i][j] = 0
        for k in range(j, n + 1):
            f[i][j] += f[i - j][k]
            
a = [None] * n

def go(pos, num, big, need):
    if num == 0:
        open('num2part.out', 'w').write('+'.join(map(str, a[:pos])))
        sys.exit()
    cur = 0
    i = big
    while cur + f[num][i] <= need:
        cur += f[num][i]
        i += 1
    a[pos] = i
    go(pos + 1, num - i, i, need - cur)

go(0, n, 1, r)
