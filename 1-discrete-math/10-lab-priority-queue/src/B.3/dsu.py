def get(v):
	p[v] = v if p[v] == v else get(p[v])
	return p[v]
def union(v1, v2):
	v1 = get(v1)
	v2 = get(v2)
	if v1 == v2: return
	m[v1], M[v1], c[v1], p[v2] = min(m[v1], m[v2]), max(M[v1], M[v2]), c[v1] + c[v2], v1
IN = open('dsu.in')
out = open('dsu.out', 'w').write
n = int(IN.readline())
m, M, c, p = range(1, n + 1), range(1, n + 1), [1] * n, range(0, n)
for ss in IN.readlines():
	s = ss.split()
	args = map(lambda s: int(s) - 1, s[1:])
	if s[0] == 'get':
		v = get(args[0])
		out('%d %d %d\n'%(m[v], M[v], c[v]))
	else:
		union(*args)
