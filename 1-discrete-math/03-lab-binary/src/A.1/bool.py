open('allvectors.out', 'w').write((lambda n: "\n".join("%%0%dd" % n % int(bin(i)[2:]) for i in range(2 ** n)))(int(open('allvectors.in').read())))
