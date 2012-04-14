n, k = map(int, open('choose.in').read().split())
out = open('choose.out', 'w').write

a = [0] * k

def go(pos, cur):
    if pos == k:
        out(' '.join(map(str, a)) + '\n')
        return 
    for i in range(cur, n):
        a[pos] = i + 1
        go(pos + 1, i + 1)

go(0, 0)
