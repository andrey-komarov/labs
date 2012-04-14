#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

#define time jsadlkda

using namespace std;

struct edge
{
	int to, id;
	edge() {}
	edge(int to, int id) : to(to), id(id) {}
};

vector<edge>* g;
vector<bool> used;
vector<int> enter;
vector<int> ret;
vector<int> color;
set<int> points;
vector<edge> stack;
int time;
int colors;

void dfs(int v, int p = -1)
{
	used[v] = true;
	enter[v] = ret[v] = time++;
	for (size_t i = 0; i < g[v].size(); i++)
	{
		edge to = g[v][i];
		if (to.to == p)
			continue;
		if (!used[to.to])
		{
			stack.push_back(to);
			dfs(to.to, v);
			if (ret[to.to] >= enter[v])
			{
				colors++;
				while (stack.back().id != to.id)
				{
					color[stack.back().id] = colors;
					stack.pop_back();
				}
				color[to.id] = colors;
				stack.pop_back();
			}
		}
		else
		{
			if (enter[to.to] < enter[v])
				stack.push_back(to);
		}
		ret[v] = min(ret[v], ret[to.to]);
	}
}

int main()
{
	ifstream in("biconv.in");
	ofstream out("biconv.out");
	int n, m;
	time = 0;
	in >> n >> m;
	g = new vector<edge>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(edge(b - 1, i));
		g[b - 1].push_back(edge(a - 1, i));
	}
	used = vector<bool>(n, false);
	ret = vector<int>(n);
	enter = vector<int>(n);
	color = vector<int>(m);
	colors = 0;
	for (int i = 0; i < n; i++)
		if (!used[i])
		{
			time = 0;
			dfs(i);
		}
	out << colors << "\n";
	for (int i = 0; i < m; i++)
		out << color[i] << " ";
	out << "\n";
}
