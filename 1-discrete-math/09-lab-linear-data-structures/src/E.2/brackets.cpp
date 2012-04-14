#include <cstdio>
#include <string>
#include <iostream>

using namespace std;

struct vector {
	struct node {
		node *next;
		char value;				
		node() {}
	};

	node *a;

	vector() {
		a = NULL;
	}
	
	void push(char b) {
		node *tmp = new node();
		tmp->next = a;
		tmp->value = b;
		a = tmp;
	}
	
	char pop() {
		char ans = a->value;
		node *tmp = a->next;
		delete a;
		a = tmp;
		return ans;
	}
	
	bool isEmpty() {
		return a == NULL;
	}
	
};

int main() {
	freopen("brackets.in", "r", stdin);
	freopen("brackets.out", "w", stdout);
	string s; 
	while (cin >> s) {
		int n = s.size();
		vector v = vector();
		bool good = true;
		for (int i = 0; i < n; i++) {
			char ch = s[i];
			if (ch == '(' || ch == '[')
				v.push(ch == '(' ? ')' : ']');
			else
				good &= !v.isEmpty() && v.pop() == ch;
		}
		good &= v.isEmpty();
		printf(good ? "YES\n" : "NO\n");
	}
}
