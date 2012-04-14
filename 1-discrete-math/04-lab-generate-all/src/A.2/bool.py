n = int(open('vectors.in').read())
ans = ["%%0%dd" % n % int(bin(i)[2:]) for i in range(2 ** n) if (lambda s: True if s.find('11') == -1 else False)("%%0%dd" % n % int(bin(i)[2:]))]
out = open('vectors.out', 'w').write
out('%d\n'%len(ans))
out('\n'.join(ans))
