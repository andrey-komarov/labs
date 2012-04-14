n = int(open('partition.in').read())
out = open('partition.out', 'w').write

a = [0] * n

def go(pos, remain, maxused):
    if remain == 0:
        out('+'.join(map(str, a[:pos])) + '\n')
    if maxused > remain:
        return
    for i in range(maxused, remain + 1):
        a[pos] = i
        go(pos + 1, remain - i, i)

go(0, n, 1)
