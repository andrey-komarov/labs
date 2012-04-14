import sys
val = map(int, open('choose2num.in').read().split())
n, k = val[:2]
a = val[2:]
a = [i - 1 for i in a]

c = [[0] * n for i in range(n)]

c[0][0] = 1
for i in range(1, n):
    for j in range(0, n):
        c[i][j] = c[i - 1][j - 1] + c[i - 1][j]
        
def go(pos, start, now):
    if pos == k:
        open('choose2num.out', 'w').write('%d'%(now))
        sys.exit(0)
    cur = 0
    for i in range(start, a[pos]):
        cur += c[n - i - 1][k - pos - 1]
    go(pos + 1, a[pos] + 1, now + cur)
    

go(0, 0, 0)

