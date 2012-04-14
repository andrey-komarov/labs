def nextsetpartition():
    read = open('nextsetpartition.in').readline
    out = open('nextsetpartition.out', 'w').write
    
    def solve():
        while solve2() == None:
            pass
    
    def final(n):
        return [[i, n - i + 1] for i in range(1, (n - 1) // 2 + 1)] + [[(n + 1)// 2] + ([] if n % 2 == 1 else [n // 2 + 1])]
    
    def can_increase(a, b):
        if len(b) == 0:
            return len(a) > 2
        if len(a) == 1 or len(a) > 2:
            return True;    
        return a[1] < b[-1]
    
    def increase(a, b):  
        if len(b) == 0:
            return [a[:-2] + [a[-1]]] + [[a[-2]]]
        if a[-1] < b[-1]:
            for i in b:
                if i > a[-1]:
                    pos = i
                    break
            ans = [a + [pos]]
        elif len(a) == 1:
            ans = [[a[0]] + [b[0]]]
        else:
            pos = len(a) - 2
            pos2 = 10**9
            for i in b:
                if i > a[pos]:
                    pos2 = i
                    break
            for i in a:
                if i < pos2 and i > a[pos]:
                    pos2 = i
            ans = [a[:pos] + [pos2]]   
        ans += [[i] for i in sorted(a + b) if i not in ans[0]]
        return ans
    
    def solve2():
        n, k = map(int, read().split())
        if (n, k) == (0, 0):
            return "Finish"
        a = [list(map(int, read().split())) for i in range(k)]
        read()
        if a == final(n):
            out('%d %d\n'%(n, n) + '\n'.join(map(str,range(1, n + 1)))+ '\n\n')
            return
        cur = []
        for i in range(k - 1, -1, -1):
            if can_increase(a[i], cur):
                ans = a[:i] + increase(a[i], cur)
                out('%d %d\n'%(n, len(ans)) + '\n'.join(' '.join(map(str, a)) for a in ans) + '\n\n')
                return
            cur += a[i]
            cur.sort()
        print 1/0
    solve()

nextsetpartition()
