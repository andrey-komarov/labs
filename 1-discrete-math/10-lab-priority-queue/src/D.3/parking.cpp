#include <cstdio>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
int n, m, n2, inf = 1000000, to;
int *t;

void put(int pos, int a) {
	pos = pos + n2 - 1;
	t[pos] = a;
	while (pos != 0) {
		pos = (pos - 1) / 2;
		t[pos] = min(t[2 * pos + 1], t[2 * pos + 2]);
	}
}

int get(int l, int r, int lb, int rb, int v) {
	if (r <= lb || rb <= l)
		return inf;
	if (lb <= l && r <= rb)
		return t[v];
	return min(
		get(l, (l + r) / 2, lb, rb, 2 * v + 1),
		get((l + r) / 2, r, lb, rb, 2 * v + 2)
	);
}

int main() {
	freopen("parking.in", "r", stdin);
	freopen("parking.out", "w", stdout);
	cin >> n >> m;
	for (n2 = 1; n2 < n; n2 *= 2);
	t = new int[2 * n2];
	for (int i = 0; i < 2 * n2; i++)
		t[i] = inf;
	for (int i = 0; i < n; i++)
		put(i, i);
	for (int t = 0; t < m; t++) {
		string s;
		cin >> s >> to;
		to--;
		if (s == "enter") {
			int a = get(0, n2, to, n2, 0);
			a = (a == inf) ? get(0, n2, 0, n2, 0) : a;
			put(a, inf);
			cout << (a + 1) << "\n";
		} else {
			put(to, to);
		}
	}
	delete[] t;
}
