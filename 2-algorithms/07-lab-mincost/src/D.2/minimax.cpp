#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cassert>

using namespace std;

struct graph2
{
	graph2(int n, int m) : n(n), m(m), g(n)
	{}

	void add_edge(int a, int b, int w)
	{	g[a].push_back(make_pair(b, w));}
	
	int matching(int min_w)
	{
		p = vector<int>(m, -1);
		int size = 0;
		for (int i = 0; i < n; i++)
		{	
			used = vector<bool>(n, false);
			size += dfs(i, min_w) ? 1 : 0;
		}	
		return size;
	}

	int n, m;
	vector<vector<pair<int, int> > > g;
	vector<bool> used;
	vector<int> p;
private:
	bool dfs(int v, int min_w)
	{
		used[v] = true;
		for (vector<pair<int, int> >::iterator it = g[v].begin(); it != g[v].end(); it++)
		{
			pair<int, int> e = *it;
			if (e.second < min_w)
				continue;
			int to = e.first;
			if (p[to] == -1 || (!used[p[to]] && dfs(p[to], min_w)))
			{
				p[to] = v;
				return true;
			}
		}
		return false;
	}
};

int main()
{
	ifstream in("minimax.in");
	ofstream out("minimax.out");
	int n, k;
	in >> n;
	graph2 g2(n, n);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			int a;
			in >> a;
			g2.add_edge(i, j, a);
		}
	int L = 0;
	int R = 1000001;
	while (R - L > 1)
	{
		int C = (L + R) / 2;
		if (g2.matching(C) != n)
			R = C;
		else
			L = C;
	}
	out << L << "\n";
}
