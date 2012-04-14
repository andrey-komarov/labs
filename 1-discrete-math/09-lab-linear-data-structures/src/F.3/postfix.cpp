#include <cstdio>

struct vector {
	struct node {
		node *next;
		int value;				
		node() {}
	};

	node *a;

	vector() {
		a = NULL;
	}
	
	void push(int b) {
		node *tmp = new node();
		tmp->next = a;
		tmp->value = b;
		a = tmp;
	}
	
	int pop() {
		int ans = a->value;
		node *tmp = a->next;
		delete a;
		a = tmp;
		return ans;
	}
};

int main() {
	freopen("postfix.in", "r", stdin);
	freopen("postfix.out", "w", stdout);
	vector v;
	char ch;
	int op;
	while (scanf("%c ", &ch) != EOF) {
		switch (ch) {
			case '+':
				v.push(v.pop() + v.pop());
				break;
			case '-':
				op = v.pop();
				v.push(v.pop() - op);
				break;
			case '*':
				v.push(v.pop() * v.pop());
				break;
			default:
				v.push(ch - '0');
		}
	}
	printf("%d", v.pop());
	if (v.a != NULL) {
		int n = 2;
		printf("%d", n / (n - 2));
	}
}
