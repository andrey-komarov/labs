#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ostream>
#include <set>

#define time ajsdljaldkf

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
set<int> bridges;
vector<int> color;
int time;

void dfs2(int v, int c)
{
        used[v] = true;
        color[v] = c;
        for (size_t i = 0; i < g[v].size(); i++)
        {
                edge to = g[v][i];
                if (!used[to.to] && !bridges.count(to.id))
                        dfs2(to.to, c);
        }
}

void dfs(int v, int last_id = -1) {
    used[v] = true;
    time++;
    enter[v] = ret[v] = time;
    for (size_t i = 0; i < g[v].size(); i++)
    {
        edge& to = g[v][i];
        if (used[to.to] && last_id != to.id)
        {
            ret[v] = min(ret[v], enter[to.to]);
        }
        if (!used[to.to])
        {
            dfs(to.to, to.id);
            ret[v] = min(ret[v], ret[to.to]);
            if (ret[to.to] > enter[v])
                bridges.insert(to.id);
        }
    }
}

int main()
{
	ifstream in("bicone.in");
	ofstream out("bicone.out");
	int n, m;
	in >> n >> m;
	g = new vector<edge>[n];
	for (int i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		g[a - 1].push_back(edge(b - 1, i + 1));
		g[b - 1].push_back(edge(a - 1, i + 1));
	}
	used = vector<bool>(n, false);
	enter = vector<int>(n, 0);
	ret = vector<int>(n, 0);
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs(i);
	used = vector<bool>(n, 0);
	color = vector<int>(n, 0);
	int colors = 0;
	for (int i = 0; i < n; i++)
		if (!used[i])
			dfs2(i, ++colors);
	out << colors << "\n";
	for (int i = 0; i < n; i++)
		out << color[i] << " ";
	out << "\n";
}
