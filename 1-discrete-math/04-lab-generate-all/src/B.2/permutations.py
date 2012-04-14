fact = lambda n: 1 if n < 2 else reduce(lambda a, b: a * b, range(1, n + 1), 1)

n = int(open('permutations.in').read())

def next_permutation(a):
    i = n - 1
    while a[i] < a[i - 1]:
        i -= 1
    b = a[i - 1]
    if i == 0:
        raise IndexError
    for j in range(n - 1, i - 1, -1):
        if a[j] > b:
            pos = j
            break
    a[pos], a[i - 1] = a[i - 1], a[pos]
    a = a[:i] + sorted(a[i:])
    return a
    
out = open('permutations.out', 'w').write
a = range(1, n + 1)
while True:
    #~ print a
    out(' '.join(map(str, a)) + '\n')
    try:
        a = next_permutation(a)
    except:
        break
    
