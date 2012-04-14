#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<int> > g;
vector<int> match;
vector<bool> used;

bool dfs(int v)
{
	used[v] = true;
	for (vector<int>::const_iterator it = g[v].begin(); it != g[v].end(); it++)
	{
		if (match[*it] == -1 || (!used[match[*it]] && dfs(match[*it])))
		{
			match[*it] = v;
			return true;
		}
	}
	return false;
}

int main()
{
	ifstream in("paths.in");
	ofstream out("paths.out");
	int n, m;
	in >> n >> m;
	match = vector<int>(n, -1);
	g.resize(n);
	for (int i = 0; i < m; i++)
	{
		int a, b;
		if (a == b)
			continue;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
	}
	int ans = 0;
	for (int i = 0; i < n; i++)
	{
		used = vector<bool>(n, false);
		if (dfs(i))
			ans++;
	}
	cerr << ans << "\n";
	out << n - ans << "\n";
}
