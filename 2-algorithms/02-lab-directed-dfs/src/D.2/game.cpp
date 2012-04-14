#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum result {WIN, LOSE, UNKNOWN};

vector<result> res;
vector<bool> used;
vector<int> *g;

void dfs(int v) 
{
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (!used[to])
			dfs(to);
	}
	bool has_lose = false;
	for (size_t i = 0; i < g[v].size(); i++)
		has_lose |= res[g[v][i]] == LOSE;
	res[v] = has_lose ? WIN : LOSE;
}

int main()
{
	ifstream in("game.in");
	ofstream out("game.out");
	int n, m, s;
	in >> n >> m >> s;
	s--;
	g = new vector<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
	}
	used = vector<bool>(n, false);
	res = vector<result>(n, UNKNOWN);
	dfs(s);
	out << (res[s] == WIN ? "First" : "Second") << " player wins\n";
}
