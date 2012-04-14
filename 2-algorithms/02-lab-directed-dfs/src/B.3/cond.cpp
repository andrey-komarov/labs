#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> *g, *g2;
vector<int> order;
vector<bool> used;
vector<int> color;

void dfs(int v)
{
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (!used[to])
			dfs(to);
	}
	order.push_back(v);
}

void dfs2(int v, int c)
{
	used[v] = true;
	color[v] = c;
	for (size_t i = 0; i < g2[v].size(); i++)
	{
		int to = g2[v][i];
		if (!used[to])
			dfs2(to, c);
	}
}

int main()
{
	ifstream in("cond.in");
	ofstream out("cond.out");
	int n, m;
	in >> n >> m;
	g = new vector<int>[n];
	g2 = new vector<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
		g2[b - 1].push_back(a - 1);
	}
	used = vector<bool>(n, false);
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs(i);
	reverse(order.begin(), order.end());
	int cnt = 0;
	used = vector<bool>(n, false);
	color = vector<int>(n, 0);
	for (int i = 0; i < n; i++)
		if (!used[order[i]])
			dfs2(order[i], cnt++);
	out << cnt << "\n";
	for (int i = 0; i < n; i++)
		out << color[i] +  1 << " ";
}
