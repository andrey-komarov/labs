import sys
n, k = map(int, open('num2brackets2.in').read().split())

dp = [[0] * (n + 2) for i in range(2 * n)]
dp[0][0] = 1
for i in range(2 * n):
    for j in range(n + 1):
        if i + j == 0:
            continue
        dp[i][j] = 0 if j == 0 else dp[i - 1][j - 1]
        dp[i][j] += dp[i - 1][j + 1]

brackets = []
a = [None] * 2 * n
def lastbracket():
    if len(brackets) == 0:
        return '=('
    else:
        return brackets[-1]

def get(bal1, bal2, pos):
    if bal1 + bal2 > 2 * n - pos:
        return 0
    return dp[2 * n - pos][bal1 + bal2] * 2 ** ((2 * n - pos - bal1 - bal2)/2)

def go(pos, bal1, bal2, need):
    if pos == 2 * n:
        open('num2brackets2.out', 'w').write(''.join(a))
        sys.exit(0)
    
    if get(bal1 + 1, bal2, pos + 1) >= need:
        brackets.append('(')
        a[pos] = '('
        go(pos + 1, bal1 + 1, bal2, need)
        
    cur = get(bal1 + 1, bal2, pos + 1)
    
    if lastbracket() == '(' and cur + get(bal1 - 1, bal2, pos + 1) >= need:
        brackets.pop()
        a[pos] = ')'
        go(pos + 1, bal1 - 1, bal2, need - cur)
        
    if lastbracket() == '(':
        cur += get(bal1 - 1, bal2, pos + 1)
        
    if get(bal1, bal2 + 1, pos + 1) + cur >= need:
        brackets.append('[')
        a[pos] = '['
        go(pos + 1, bal1, bal2 + 1, need - cur)
    
    cur += get(bal1, bal2 + 1, pos + 1)
    
    if lastbracket() == '[' and get(bal1, bal2 - 1, pos + 1) + cur >= need:
        brackets.pop()
        a[pos] = ']'
        go(pos + 1, bal1, bal2 - 1, need - cur)
    
    if lastbracket() == '[':
        cur += get(bal1, bal2 - 1, pos + 1)
    
    sys.exit(1)
    
#~ print get(1,0,7)
go(0, 0, 0, k + 1)

