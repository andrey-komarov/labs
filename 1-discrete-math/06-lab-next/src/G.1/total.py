def nextbrackets():
    open('nextbrackets.out', 'w').write(\
        (\
            lambda s: \
                (s != ('()' * (len(s) / 2))) \
                    and \
                (\
                    lambda l: \
                        s[:l + 1] + ')' + \
                        '(' * ((len(s) - l - 1) / 2) + \
                        ')' * ((len(s) - l - 1) / 2)\
                )(\
                    s.rfind('((')\
                ) \
                    or \
                '-'\
        )(\
            open('nextbrackets.in').read().strip()\
        )\
    )

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
    c == range(1, n + 1)[k:] and (out('-1')or True) or out(' '.join(map(str, next(c))))

def nextmultiperm():
    longest_monotone_sequence = lambda p, comp: \
        reduce(\
            lambda (length, flag, last), now:\
                (length + 1, True, now) \
                    if flag and comp(last, now)<=0 else \
                (length, False, now),\
            p,
            (0, True, p[0])
        )[0]
    
    get_last = lambda p, comp, num: \
        reduce(\
            lambda index, now: index if comp(num, p[now]) >= 0 else now,\
            range(0, len(p)),\
            0\
        )
    
    rebuild = lambda p, comp: \
        (lambda l:\
            [p[l]] + sorted(p[:l] + p[l + 1:], cmp=comp)\
        )\
        (\
            get_last(p, comp, p[0])\
        )
            
    
    next_perm2 = lambda p, comp: (\
        lambda l:\
            p[:-l - 1] + rebuild(p[-l - 1:], comp)\
        )(longest_monotone_sequence(p[::-1], comp))
        
    
    next_perm = lambda p, o = 1: \
        [0] * len(p) \
            if p == sorted(p)[::-o] else \
        next_perm2(\
            p, (\
                lambda a, b: o * (a - b)\
            )\
        )
    
    solve = lambda p: \
        ' '.join(\
            map(\
                str, \
                next_perm(p)\
            )\
        )
    
    open('nextmultiperm.out', 'w').write(\
        solve(\
            map(\
                int, \
                open('nextmultiperm.in').read().split()\
            )[1:]\
        )\
    )

def nextpartition():
    def split(a, b):
        return [b for i in range(a // b - 1) ] + [a % b + b]
    n, p = open('nextpartition.in').read().strip().split('=')
    out = open('nextpartition.out', 'w').write
    p = map(int, p.split('+'))
    if len(p) == 1:
        out('No solution')
    elif p[-1] - p[-2] > 1:
        out(n + '=' + '+'.join(map(str, p[:-2] + [p[-2] + 1] + split(p[-1] - 1, p[-2] + 1))))
    else:
        out(n + '=' + '+'.join(map(str, p[:-2] + [p[-1] + p[-2]])))
    
def nextperm():
    longest_monotone_sequence = lambda p, comp: \
        reduce(\
            lambda (length, flag, last), now:\
                (length + 1, True, now) \
                    if flag and comp(last, now)<=0 else \
                (length, False, now),\
            p,
            (0, True, p[0])
        )[0]
    
    get_last = lambda p, comp, num: \
        reduce(\
            lambda index, now: index if comp(num, p[now]) >= 0 else now,\
            range(0, len(p)),\
            0\
        )
    
    rebuild = lambda p, comp: \
        (lambda l:\
            [p[l]] + sorted(p[:l] + p[l + 1:], cmp=comp)\
        )\
        (\
            get_last(p, comp, p[0])\
        )
            
    
    next_perm2 = lambda p, comp: (\
        lambda l:\
            p[:-l - 1] + rebuild(p[-l - 1:], comp)\
        )(longest_monotone_sequence(p[::-1], comp))
        
    
    next_perm = lambda p, o = 1: \
        [0] * len(p) \
            if p == range(1, len(p) + 1)[::-o] else \
        next_perm2(\
            p, (\
                lambda a, b: o * (a - b)\
            )\
        )
    
    solve = lambda p: \
        ' '.join(\
            map(\
                str, \
                next_perm(p, -1)\
            )\
        ) + \
        '\n' + \
        ' '.join(\
            map(\
                str, \
                next_perm(p)\
            )\
        )
    
    open('nextperm.out', 'w').write(\
        solve(\
            map(\
                int, \
                open('nextperm.in').read().split()\
            )[1:]\
        )\
    )
    
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
        return a[1] < b[0]
    
    def increase(a, b):    
        if len(b) == 0:
            return [a[:-2] + [a[-1]]] + [[a[-2]]]
        if a[-1] < b[-1]:
            for i in b:
                if i > a[-1]:
                    pos = i
                    break
            ans = [a + [pos]]
        else:
            pos = 0
            for i in range(len(a) - 1, -1, -1):
                if a[i] < b[-1]:
                    pos = i
                    break
            pos2 = 0
            for i in b:
                if i > a[pos]:
                    pos2 = i
                    break
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
        #~ print a
        cur = []
        for i in range(k - 1, -1, -1):
            if can_increase(a[i], cur):
                ans = a[:i] + increase(a[i], cur)
                out('%d %d\n'%(n, len(ans)) + '\n'.join(' '.join(map(str, a)) for a in ans) + '\n\n')
                return
            cur += a[i]
        
    solve()
    
def nextvector():
    open('nextvector.out', 'w').write(\
        (\
            lambda s, l, g: \
                g(s, l, '0', '1') + '\n' + g(s, l, '1', '0')\
            )\
        (\
            open('nextvector.in').read().strip(), \
            (\
                lambda s, c: \
                    reduce(\
                        lambda (length, flag), char: \
                            (length + 1, True) \
                                if flag and char == c else \
                            (length, False), \
                        s, \
                        (0, True) \
                    )[0]\
            ), \
            (\
                lambda s, l, o, z: \
                    (\
                        lambda l: \
                            s[:-l-1] + o + z * l\
                    )\
                    (\
                        l(s[::-1], o), \
                    ) if z in s else\
                        '-'\
            )\
        )\
    )
    
tasks = [nextvector, nextsetpartition, nextperm, nextpartition, nextmultiperm, nextbrackets, nextchoose]

for task in tasks:
    try:
        task()
    except:
        pass

