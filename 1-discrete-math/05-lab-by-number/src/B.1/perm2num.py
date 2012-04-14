import sys
val = map(int, open('perm2num.in').read().split())
n = val[0]
a = val[1:]
a = [i - 1 for i in a]
b = [0] * n
f = lambda n: 1 if n < 2 else f(n - 1) * n
fact = [f(i) for i in range(n + 1)]
used = [False] * n

def go(pos, now):
    if pos == n:
        open('perm2num.out', 'w').write('%d'%now)
        sys.exit(0)
    cnt = a[pos] - sum(b[i] for i in range(a[pos])) 
    #~ print cnt
    b[a[pos]] = 1
    go(pos + 1, now + cnt * fact[n - pos - 1])

go(0, 0)
