#include <cstdio>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T> struct vector {
	T *a;
	int l, cnt;
	static const int init_len = 8;
		
	vector() {
		a = new T[init_len];
		l = init_len;
		cnt = 0;
	}
	
	void push(T val) {
		if (l == cnt) {
			T *b = new T[l * 2];
			for (int i = 0; i < cnt; i++) {
				b[i] = a[i];
			}
			T *tmp = a;
			a = b;
			delete[] tmp;
			l *= 2;
		}
		a[cnt++] = val;
	}
	
	T pop() {
		if ((cnt < l / 4) && cnt > init_len) {
			T *b = new T[l / 2];
			for (int i = 0; i < cnt; i++) {
				b[i] = a[i];
			}
			T *tmp = a;
			a = b;
			delete tmp;
			l /= 2;
		}
		return a[--cnt];
	}
	
	T& operator[](int pos) {
		return a[pos];
	}
};

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

vector<string> program;
//~ map<string, int> pos;
int *jump;
int registers[26];
queue q;
string *labels;

int n;

int main() {
	freopen("quack.in", "r", stdin);
	freopen("quack.out", "w", stdout);
	string s;
	while (cin >> s)
		program.push(s);
	n = program.cnt;
	jump = new int[n];
	labels = new string[n];

	for (int i = 0; i < n; i++) {
		string s = program[i];
		if (s[0] == ':') {
			labels[i] = s.substr(1, s.size() - 1);
		} else {
			labels[i] = "nu ne budet vo vhodnom faile takoy metki :)";
		}
	}
	
	for (int i = 0; i < n; i++) {
		string s = program[i];
		string search;
		bool need_search = false;
		if (s[0] == 'J') {
			search = s.substr(1, s.length() - 1);
			need_search = true;
		}
		if (s[0] == 'Z') {
			search = s.substr(2, s.length() - 2);
			need_search = true;
			//~ jump[i] = pos[s.substr(2, s.length() - 2)];
		}
		if (s[0] == 'E' || s[0] == 'G') {
			search = s.substr(3, s.length() - 3);
			need_search = true;
			//~ jump[i] = pos[s.substr(3, s.length() - 3)];
		}
		if (need_search) {
			for (int j = 0; j < n; j++) {
				if (labels[j] == search) {
					jump[i] = j;
					break;
				}
			}
		}
	}
	
	const int mod = 65536;
	int pos = 0;
	while (pos != n) {
		string s = program[pos];
		int x, y;
		switch (s[0]) {
			case '+':
				q.push((q.pop() + q.pop() + mod) % mod);
				break;
			case '-':
				q.push((q.pop() - q.pop() + mod) % mod);
				break;
			case '*':
				q.push((int)(((long long)q.pop() * q.pop()) % mod));
				break;
			case '/':
				x = q.pop();
				y = q.pop();
				q.push( y == 0 ? 0 : (x / y) );
				break;
			case '%':
				x = q.pop();
				y = q.pop();
				q.push( y == 0 ? 0 : (x % y) );
				break;
			case '>':
				registers[s[1] - 'a'] = q.pop();
				break;
			case '<':
				q.push(registers[s[1] - 'a']);
				break;
			case 'P':
				if (s.size() == 1) 
					printf("%d\n", q.pop());
				else 
					printf("%d\n", registers[s[1] - 'a']);
				break;
			case 'C':
				if (s.size() == 1)
					printf("%c", q.pop() % 256);
				else 
					printf("%c", registers[s[1] - 'a'] % 256);
				break;
			case ':':
				break;
			case 'J':
				pos = jump[pos] - 1;
				break;
			case 'Z':
				if (registers[s[1] - 'a'] == 0)
					pos = jump[pos] - 1;
				break;
			case 'E':
				if (registers[s[1] - 'a'] == registers[s[2] - 'a'])
					pos = jump[pos] - 1;
				break;
			case 'G':
				if (registers[s[1] - 'a'] > registers[s[2] - 'a'])
					pos = jump[pos] - 1;
				break;
			case 'Q':
				return 0;
			default:
				q.push(atoi(s.c_str()));
		}
		pos++;
	}
}
