print >> open('isheap.out', 'w'), "YES" if (lambda a: all(a[(i - 1) // 2] <= a[i] for i in range(1, len(a))))(map(int, open('isheap.in').read().split())[1:]) else "NO"
