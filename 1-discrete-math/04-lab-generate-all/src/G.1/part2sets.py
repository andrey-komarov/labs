n, k = map(int, open('part2sets.in').read().split())
out = open('part2sets.out', 'w').write

a = [[0] * n for i in range(k)]
l = [1] * k
used = [0] * (n + 1)

def go(free, maxused):
    if free == 0:
        out('\n'.join(' '.join(map(str, a[i][:l[i]])) for i in range(k)) + '\n\n')
        return
    for i in range(k):
        for j in range(max(maxused, a[i][l[i] - 1]) + 1, n + 1):
            if used[j]:
                continue
            a[i][l[i]] = j
            l[i] += 1
            go(free - 1, j)
            l[i] -= 1
            a[i][l[i]] = 0

def init(pos, cur):
    if pos == k:
        go(n - k, 0)
        return 
    for i in range(cur, n):
        a[pos][0] = i + 1
        used[i + 1] = True
        init(pos + 1, i + 1)
        used[i + 1] = False

init(0, 0)
