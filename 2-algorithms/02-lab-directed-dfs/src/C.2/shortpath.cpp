#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> order;
vector<bool> used;
vector<int> color;

struct edge
{
	int to, w;
	edge() {}
	edge(int to, int w) : to(to), w(w) {}
};

vector<edge> *g;

void dfs(int v)
{
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i].to;
		if (!used[to])
			dfs(to);
	}
	order.push_back(v);
}

int main()
{
	ifstream in("shortpath.in");
	ofstream out("shortpath.out");
	int n, m, s, f;
	in >> n >> m >> s >> f;
	s--;
	f--;
	g = new vector<edge>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b, w;
		in >> a >> b >> w;
		g[a - 1].push_back(edge(b - 1, w));
	}
	used = vector<bool>(n, false);
	dfs(s);
	reverse(order.begin(), order.end());
	vector<int> d(n, 1 << 30);
	d[order[0]] = 0;
	for (size_t i = 0; i < order.size(); i++)
	{
		int now = order[i];
		for (size_t j = 0; j < g[now].size(); j++)
		{
			edge to = g[now][j];
			if (d[to.to] > d[now] + to.w)
				d[to.to] = d[now] + to.w;
		}
	}
	if (d[f] == 1<<30)
		out << "Unreachable\n";
	else
		out << d[f] << "\n";
	
}
