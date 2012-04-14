#include <cstdio>

struct vector {
	int *a, *m;
	int l, cnt;
	static const int init_len = 8;
		
	vector() {
		a = new int[init_len];
		m = new int[init_len];
		l = init_len;
		cnt = 0;
	}
	
	void push(int val) {
		if (l == cnt) {
			int *b = new int[l * 2];
			int *c = new int[l * 2];
			for (int i = 0; i < cnt; i++) {
				b[i] = a[i];
				c[i] = m[i];				
			}
			int *tmp = a;
			a = b;
			delete tmp;
			tmp = m;
			m = c;
			delete tmp;
			l *= 2;
		}
		a[cnt] = val;
		m[cnt] = (cnt == 0) ? val : m[cnt - 1];
		if (m[cnt] > val) {
			m[cnt] = val;
		}
		cnt++;
	}
	
	int pop() {
		if ((cnt < l / 4) && cnt > init_len) {
			int *b = new int[l / 2];
			int *c = new int[l / 2];
			for (int i = 0; i < cnt; i++) {
				b[i] = a[i];
				c[i] = m[i];				
			}
			int *tmp = a;
			a = b;
			delete tmp;
			tmp = m;
			m = c;
			delete tmp;
			l /= 2;
		}
		return a[--cnt];
	}
	
	int min() {
		return cnt == 0 ? 2147000000 : m[cnt - 1];
	}
};

struct queue {
	
	vector head, tail;
		
	queue() {
	}
	
	void push(int value) {
		tail.push(value);
	}
	
	int pop() {
		if (!head.cnt) {
			while (tail.cnt)
				head.push(tail.pop());
		}
		return head.pop();
	}
	
	int min() {
		int m1 = head.min(), m2 = tail.min();
		return m1 < m2 ? m1 : m2;
	}
};

int main() {
	freopen("queuemin.in", "r", stdin);
	freopen("queuemin.out", "w", stdout);
	int n;
	scanf("%d\n", &n);
	queue v;
	for (int i = 0; i < n; i++) {
		char type;
		scanf("%c", &type);
		if (type == '-') {
			v.pop();
		} else if (type == '?') {
			printf("%d\n", v.min());
		} else {
			int a;
			scanf("%d", &a);
			v.push(a);			
		}
		scanf("\n");
	}
}


