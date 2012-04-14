class vector:
	def __init__(self):
		self.l = 8
		self.cnt = 0
		self.a = [0] * self.l
		
	def push(self, new):
		if self.cnt == self.l:
			b = [0] * (2 * self.l)
			b[:self.l] = self.a
			self.a = b
			self.l *= 2
		self.a[self.cnt] = new
		self.cnt += 1
	
	def pop(self):
		if self.cnt < self.l // 4 and self.cnt > 4:
			self.l //= 2
			self.a = self.a[:self.l] 
		self.cnt -= 1
		return self.a[self.cnt]

	def top(self):
		return self.a[self.cnt - 1]

next = open('stack1.in').readline
out = open('stack1.out', 'w').write
n = int(next())
a = vector()

for i in xrange(n):
	s = next()
	if s[0] == '-':
		out(a.pop())
	else:
		a.push(s[2:])
