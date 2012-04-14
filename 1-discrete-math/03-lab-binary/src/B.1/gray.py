open('gray.out', 'w').write((lambda n: "\n".join("%s" % "%%0%dd" % n % int(bin(i ^ (i >> 1))[2:]) for i in range(2 ** n)))(int(open('gray.in').read())))
