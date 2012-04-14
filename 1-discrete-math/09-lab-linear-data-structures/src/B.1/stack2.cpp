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
	freopen("stack2.in", "r", stdin);
	freopen("stack2.out", "w", stdout);
	int n;
	scanf("%d\n", &n);
	vector v;
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
