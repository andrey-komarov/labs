import sys
a = open('brackets2num2.in').read().strip()
n = len(a) / 2

dp = [[0] * (n + 2) for i in range(2 * n)]
dp[0][0] = 1
for i in range(2 * n):
    for j in range(n + 1):
        if i + j == 0:
            continue
        dp[i][j] = 0 if j == 0 else dp[i - 1][j - 1]
        dp[i][j] += dp[i - 1][j + 1]

def get(bal1, bal2, pos):
    if bal1 + bal2 > 2 * n - pos:
        return 0
    return dp[2 * n - pos][bal1 + bal2] * 2 ** ((2 * n - pos - bal1 - bal2)/2)

brackets = []
def lastbracket():
    if len(brackets) == 0:
        return '=('
    else:
        return brackets[-1]

def go(pos, bal1, bal2, now):
    if pos == 2 * n:
        open('brackets2num2.out', 'w').write('%d'%now)
        sys.exit(0)
    cur = 0
    if a[pos] == '(':
        brackets.append('(')
        go(pos + 1, bal1 + 1, bal2, now)
    cur += get(bal1 + 1, bal2, pos + 1)
    
    if a[pos] == ')' and lastbracket() == '(':
        brackets.pop()
        go(pos + 1, bal1 - 1, bal2, now + cur)
    if bal1 != 0 and lastbracket() == '(':
        cur += get(bal1 - 1, bal2, pos + 1)
        
    if a[pos] == '[':
        brackets.append('[')
        go(pos + 1, bal1, bal2 + 1, now + cur)
    cur += get(bal1, bal2 + 1, pos +1)
    
    if a[pos] == ']' and lastbracket() == '[':
        brackets.pop()
        go(pos + 1, bal1, bal2 - 1, now + cur)
        
    print ":-("
go(0, 0, 0, 0)
