def nextchoose():
    def next(c):
        pos = k - 1
        while c[pos] == n - k + 1 + pos:
            pos -= 1
        c[pos] += 1
        return c[:pos + 1] + range(c[pos] + 1, c[pos] + k - pos)
    
    with open('nextchoose.in') as inf:
        n, k = map(int, inf.readline().split())
        c = map(int, inf.readline().split())
    
    out = open('nextchoose.out', 'w').write
    c == range(1, n + 1)[-k:] and (out('-1')or True) or out(' '.join(map(str, next(c))))

nextchoose()
