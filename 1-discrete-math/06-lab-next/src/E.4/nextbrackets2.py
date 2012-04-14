def nextbrackets():
    s = open('nextbrackets.in').read().strip()
    n = len(s) // 2
    d = {'(':-1, ')':1}
    bal = 0
    for i in range(len(s))[::-1]:
        bal += d[s[i]]
        if s[i] == '(' and 2 * n - i - 1 >= bal + 1 and bal > 0:
            s2 = s[:i] + ')' + '(' * ((2 * n - i - bal) / 2) + ')' * ((2 * n - i + bal - 1) / 2)
            break
    with open('nextbrackets.out', 'w') as ouf:
        try:
            ouf.write(s2)
        except:
            ouf.write('-')
    
nextbrackets()
