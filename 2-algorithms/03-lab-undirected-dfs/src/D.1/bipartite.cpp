#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>

using namespace std;

vector<int>* g;
vector<bool> used;
vector<int> color;
string ans;

void dfs(int v, int c = 1)
{
	used[v] = true;
	color[v] = c;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (color[to] != 0 && color[to] == color[v])
			ans = "NO";
		if (!used[to])
			dfs(to, -c);
	}
}

int main()
{
	ifstream in("bipartite.in");
	ofstream out("bipartite.out");
	int n, m;
	in >> n >> m;
	g = new vector<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
		g[b - 1].push_back(a - 1);
	}
	ans = "YES";
	used = vector<bool>(n, false);
	color = vector<int>(n, 0);	
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs(i);
	out << ans << "\n";
}
