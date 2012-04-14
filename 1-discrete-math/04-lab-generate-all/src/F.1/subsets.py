n = int(open('subsets.in').read())
out = open('subsets.out', 'w').write

a = [0] * n

def go(pos, last):
    if last == n + 1:
        return
    out(' '.join(str(a[i]) for i in range(n) if a[i] != 0) + '\n')
    for i in range(last + 1, n + 1):
        a[pos] = i
        go(pos + 1, i)
        a[pos] = 0
    
go(0, 0)
