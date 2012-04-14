import sys
a = map(int, open('part2num.in').read().split('+'))
n = sum(a)

f = [[0] * (n + 1) for i in range(n + 1)]
for i in range(n + 1):
    f[i][i] = 1
for i in range(n + 1):
    for j in range(1, i):
        f[i][j] = 0
        for k in range(j, n + 1):
            f[i][j] += f[i - j][k]

def go(pos, num, big, now):
    if num == 0:
        open('part2num.out', 'w').write('%d'%now)
        sys.exit()
    cur = 0
    i = big
    for i in range(big, a[pos]):
        cur += f[num][i]
    go(pos + 1, num - a[pos], a[pos], now + cur)

go(0, n, 1, 0)

