#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

#define time jsadlkda

using namespace std;

vector<int>* g;
vector<bool> used;
vector<int> enter;
vector<int> ret;
set<int> points;
int time;

void dfs(int v, int p = -1)
{
	used[v] = true;
	time++;
	enter[v] = ret[v] = time;
	int cnt = 0;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		int to = g[v][i];
		if (used[to] && to != p)
			ret[v] = min(ret[v], enter[to]);
		if (!used[to])
		{
			dfs(to, v);
			cnt++;
			ret[v] = min(ret[v], ret[to]);
			if (ret[to] >= enter[v] && p != -1) 
			{
				points.insert(v);
			}
		}
	}
	if (p == -1 && cnt > 1)
		points.insert(v);
}

int main()
{
	ifstream in("points.in");
	ofstream out("points.out");
	int n, m;
	time = 0;
	in >> n >> m;
	g = new vector<int>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(b - 1);
		g[b - 1].push_back(a - 1);
	}
	used = vector<bool>(n, false);
	ret = vector<int>(n);
	enter = vector<int>(n);
	for (int i = 0; i < n; i++)
		if (!used[i])
		{
			time = 0;
			dfs(i);
		}
	out << points.size() << "\n";
	for (set<int>::iterator it = points.begin(); it != points.end(); it++)
		out << (*it) + 1 << " ";
	out << "\n";
}
