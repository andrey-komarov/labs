#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

set<int>* g;
vector<int> order;
vector<bool> used;

void dfs(int v)
{
	used[v] = true;
	for (set<int>::const_iterator it = g[v].begin(); it != g[v].end(); it++)
	{
		int to = *it;
		if (!used[to])
			dfs(to);
	}
	order.push_back(v);
}

int main()
{
	ifstream in("hamiltonian.in");
	ofstream out("hamiltonian.out");
	int n, m;
	in >> n >> m;
	g = new set<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].insert(b - 1);
	}
	used = vector<bool>(n, false);
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs(i);
	reverse(order.begin(), order.end());
	for (int i = 0; i < n - 1; i++)
		if (!g[order[i]].count(order[i + 1]))
		{
			out << "NO\n";
			return 0;
		}
	out << "YES\n";
}

