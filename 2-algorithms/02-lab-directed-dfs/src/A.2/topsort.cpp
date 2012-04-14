#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int>* g;
vector<int> order;
vector<bool> used;
vector<int> color;

bool has_cycle(int v)
{
	color[v] = 1;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (color[to] == 1)
			return true;
		if (color[to] == 0)
			if (has_cycle(to))
				return true;
	}
	color[v] = 2;
	return false;
}

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

int main()
{
	ifstream in("topsort.in");
	ofstream out("topsort.out");
	int n, m;
	in >> n >> m;
	g = new vector<int>[n];
	for (int i = 0; i < n; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
	}
	used = vector<bool>(n, false);
	color = vector<int>(n, 0);	
	for (int i = 0; i < n; i++)
		if (color[i] == 0)
			if (has_cycle(i))
			{
				out << "-1\n";
				return 0;
			}
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs(i);
	reverse(order.begin(), order.end());
	for (int i = 0; i < n; i++)
		out << order[i] +  1 << " ";
}
