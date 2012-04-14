import sys
n, k = map(int, open('num2perm.in').read().split())

a = [0] * n
used = [False] * n
f = lambda n: 1 if n < 2 else f(n - 1) * n

fact = [f(i) for i in range(n + 1)]

def go(pos, left):
    if pos == n:
        open('num2perm.out', 'w').write(' '.join(map(str, [i + 1 for i in a])))
        sys.exit(0)        
    m = n - pos - 1
    cur = 0
    i = 0
    while (cur + fact[m] <= left) or used[i]:
        if used[i]:
            i += 1
            continue
        i += 1
        cur += fact[m]
    a[pos] = i
    used[i] = True
    go(pos + 1, left - cur)
    used[i] = False

go(0, k)
