#include <cstdio>

struct queue {
	struct node {
		node *next;
		int value;				
		node() {}
	};

	node *head, *tail;

	queue() {
		head = tail = NULL;
	}

	void push(int b) {
		if (head == NULL) {
			tail = new node();
			head = tail;
			tail->value = b;
			tail->next = NULL;
		} else {
			node *tmp = new node();
			tmp->value = b;
			tmp->next = NULL;
			tail->next = tmp;
			tail = tmp;
		}
	}

	int pop() {
		int ans = head->value;
		node *tmp = head->next;
		delete head;
		head = tmp;
		return ans;
	}
};

int main() {
	freopen("queue2.in", "r", stdin);
	freopen("queue2.out", "w", stdout);
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
