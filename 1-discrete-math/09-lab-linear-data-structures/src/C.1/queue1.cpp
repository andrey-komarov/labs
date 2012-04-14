#include <cstdio>

struct queue {
	
	int *a;
	int p1, p2;
	int l;
	static const int init_len = 8;
	
	queue() {
		a = new int[init_len];
		p1 = p2 = 0;
		l = init_len;
	}
	
	void push(int value) {
		if (p2 == l) {
			int *b = new int[l * 2];
			for (int i = 0; i < l; i++) {
				b[i] = a[i];
			}
			l *= 2;
			int *c = a;
			a = b;
			delete c;
		}
		a[p2++] = value;
	}
	
	int pop() {
		if (p1 > l / 2 && l > init_len) {
			int *b = new int[l / 2];
			for (int i = p1; i < p2; i++) {
				b[i - p1] = a[i];
			}
			l /= 2;
			int *c = a;
			a = b;
			delete c;
			p2 -= p1;
			p1 -= p1;
		}
		return a[p1++];
	}
};

int main() {
	freopen("queue1.in", "r", stdin);
	freopen("queue1.out", "w", stdout);
	int n;
	scanf("%d\n", &n);
	queue v;
	for (int i = 0; i < n; i++) {
		char type;
		scanf("%c", &type);
		if (type == '-') {
			printf("%d\n", v.pop());
		} else {
			int a;
			scanf("%d", &a);
			v.push(a);			
		}
		scanf("\n");
	}
}

