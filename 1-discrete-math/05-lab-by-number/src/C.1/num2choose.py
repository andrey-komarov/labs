import sys
n, k, m = map(int, open('num2choose.in').read().split())

c = [[0] * n for i in range(n)]

c[0][0] = 1
for i in range(1, n):
    for j in range(0, n):
        c[i][j] = c[i - 1][j - 1] + c[i - 1][j]
        
a = [None] * k

def go(pos, left, start = -1):
    if pos == k:
        open('num2choose.out', 'w').write(' '.join(map(str, [i + 1 for i in a])))
        sys.exit(0)                
    cur = 0
    i = start + 1
    m = k - pos - 1
    while cur + c[n - i - 1][m] < left:
        cur += c[n - i - 1][m]
        i += 1
    a[pos] = i
    go(pos + 1, left - cur, i)
    
go(0, m + 1)
