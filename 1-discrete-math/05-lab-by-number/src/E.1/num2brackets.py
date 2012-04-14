import sys
n, k = map(int, open('num2brackets.in').read().split())

dp = [[0] * (n + 2) for i in range(2 * n)]

dp[0][0] = 1

for i in range(2 * n):
    for j in range(n + 1):
        if i + j == 0:
            continue
        dp[i][j] = 0 if j == 0 else dp[i - 1][j - 1]
        dp[i][j] += dp[i - 1][j + 1]

a = [None] * 2 * n

def go(pos, bal, need):
    if pos == 2 * n:
        open('num2brackets.out', 'w').write(''.join(a))
        sys.exit(0)
    if dp[2 * n - pos - 1][bal + 1] <= need:
        a[pos] = ')'
        go(pos + 1, bal - 1, need - dp[2 * n - pos - 1][bal + 1])
    else:
        a[pos] = '('
        go(pos + 1, bal + 1, need)

go(0, 0, k)
