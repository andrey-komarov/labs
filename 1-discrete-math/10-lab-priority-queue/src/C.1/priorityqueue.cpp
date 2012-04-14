#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

void swapp(int, int);

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
			delete[] tmp;
			l /= 2;
		}
		return a[--cnt];
	}
	
	T& operator[](int pos) {
		return a[pos];
	}
};

vector<int> q, where, id;
int line;

void up(int v) {
	if (v == 0) 
		return;	
	int to = (v - 1) / 2;
	if (q[to] > q[v]) {
		swapp(v, to);
		up(to);
	}
}

void down(int v) {
	int m = q[v];
	if (2 * v + 1 < q.cnt) {
		m = min(m, q[2 * v + 1]);
	}
	if (2 * v + 2 < q.cnt) {
		m = min(m, q[2 * v + 2]);
	}
	if (m == q[v])
		return;
	if (m == q[2 * v + 1]) {
		swapp(2 * v + 1, v);
		down(2 * v + 1);
		return;
	}
	swapp(2 * v + 2, v);
	down(2 * v + 2);
}

void swapp(int v1, int v2) {
	where[id[v1]] = v2;
	where[id[v2]] = v1;
	int tmp = q[v1];
	q[v1] = q[v2];
	q[v2] = tmp;
	tmp = id[v1];
	id[v1] = id[v2];
	id[v2] = tmp;
}

void push(int a) {
	q.push(a);
	where[line] = q.cnt - 1;
	id.push(line);
	up(q.cnt - 1);
}

void extract_min(ostream& out) {
	if (q.cnt == 0) {
		out << "*\n";
		return;
	}
	out << q[0] << "\n";
	swapp(0, q.cnt - 1);
	q.pop();
	id.pop();
	down(0);
}

void decrease_key(int a, int new_key) {
	q[where[a]] = new_key;
	up(where[a]);
	down(where[a]);
}

template<typename T> 
ostream& operator<<(ostream& out, vector<T> a) {
	out << "[";
	for (int i = 0; i < a.cnt; i++)
		out << a[i] << ", ";
	out << "]";
	return out;
}

int main() {
	freopen("priorityqueue.in", "r", stdin);
	freopen("priorityqueue.out", "w", stdout);
	string s;
	where.push(-1);
	while (cin >> s) {
		line++;
		where.push(-1);
		if (s == "push") {
			int n;
			cin >> n;
			push(n); 
		} else if (s == "extract-min") {
			extract_min(cout);
		} else {
			int pos, new_key;
			cin >> pos >> new_key;
			decrease_key(pos, new_key);
		}
	}
}
