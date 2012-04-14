n = int(open('brackets.in').read())
out = open('brackets.out', 'w').write

a = [None] * 2 * n

def go(pos, opened):
    if opened < 0 or pos + opened > 2 * n:
        return
    if opened == 0 and pos == 2 * n:
        out(''.join(a) + '\n')
        return 
    a[pos] = '('
    go(pos + 1, opened + 1)
    a[pos] = ')'
    go(pos + 1, opened - 1)
    
go(0, 0)
