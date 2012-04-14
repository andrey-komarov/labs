import sys
a = open('brackets2num.in').read().strip()
n = len(a) / 2

dp = [[0] * (n + 2) for i in range(2 * n)]

dp[0][0] = 1

for i in range(2 * n):
    for j in range(n + 1):
        if i + j == 0:
            continue
        dp[i][j] = 0 if j == 0 else dp[i - 1][j - 1]
        dp[i][j] += dp[i - 1][j + 1]

def go(pos, bal, now):
    if pos == 2 * n:
        open('brackets2num.out', 'w').write('%d'%now)
        sys.exit(0)
    if a[pos] == ')':
        go(pos + 1, bal - 1, now + dp[2 * n - pos - 1][bal + 1])
    else:
        go(pos + 1, bal + 1, now)


go(0, 0, 0)
