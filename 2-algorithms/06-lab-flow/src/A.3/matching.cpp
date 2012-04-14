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
	ifstream in("matching.in");
	ofstream out("matching.out");
	int n, k, m;
	in >> n >> k >> m;
	match = vector<int>(k, -1);
	g.resize(n);
	for (int i = 0; i < m; i++)
	{
		int a, b;
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
	out << ans << "\n";
}
