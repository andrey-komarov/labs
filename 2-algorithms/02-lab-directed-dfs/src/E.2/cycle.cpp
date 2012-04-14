#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

vector<int>* g;
vector<int> color;
vector<int> path;

void output_cycle(int v, ostream& out) 
{
	bool started = false;
	out << "YES\n";
	for (size_t i = 0; i < path.size(); i++)
	{
		if (path[i] == v)
			started = true;
		if (started)
			out << path[i] + 1 << " ";
	}
	out << endl;
	exit(0);
}

bool dfs(int v, ostream& out)
{
	path.push_back(v);
	color[v] = 1;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (color[to] == 1)
			output_cycle(to, out);
		if (color[to] == 0)
			dfs(to, out);
	}
	color[v] = 2;
	path.pop_back();
	return false;
}

int main()
{
	ifstream in("cycle.in");
	ofstream out("cycle.out");
	int n, m;
	in >> n >> m;
	g = new vector<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
	}
	color = vector<int>(n, 0);
	for (int i = 0; i < n; i++)
		if (color[i] == 0)
			dfs(i, out);
	out << "NO\n";
}
