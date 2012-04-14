net = [
	[(1, 2), (3, 4), (5, 6), (7, 8), (9, 10), (11, 12), (13, 14), (15, 16)], 
	[(1, 3), (2, 4), (5, 7), (6, 8), (9, 11), (10, 12), (13, 15), (14, 16)],
	[(1, 5), (2, 6), (3, 7), (4, 8), (9, 13), (10, 14), (11, 15), (12, 16)],
	[(1, 9), (2, 10), (3, 11), (4, 12), (5, 13), (6, 14), (7, 15), (8, 16)],
	[(2, 3), (4, 13), (5, 9), (6, 11), (7, 10), (8, 12), (14, 15)],
	[(2, 5), (3, 9), (6, 7), (8, 14), (10, 11), (12, 15)],
	[(3, 5), (4, 9), (8, 13), (10, 11), (12, 14)],
	[(4, 6), (7, 9), (8, 10), (11, 13)],
	[(4, 5), (6, 7), (8, 9), (10, 11), (12, 13)],
	[(7, 8), (9, 10)]
]
n = int(open('netbuild.in').read())
banned = range(n + 1, 17)

def not_banned(a):
	return a[0] not in banned and a[1] not in banned

def fix(l):
	return filter(not_banned, l)

net = [fix(l) for l in net]
net = filter(lambda l: True if l else False, net)

out = open('netbuild.out', 'w').write
out('%d %d %d\n'%(n, sum(len(l) for l in net), len(net)))
for l in net:
	out('%d '%len(l) + ''.join('%d %d '%(c[0], c[1]) for c in l) + '\n')
