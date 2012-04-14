#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ostream>

#define time ajsdljaldkf

using namespace std;

struct edge
{
	int to, id;
	edge() {}
	edge(int to, int id) : to(to), id(id) {}
	bool operator<(const edge& e) const
	{
		return id < e.id;
	}
};

vector<edge>* g;
vector<bool> used;
vector<int> enter;
vector<int> ret;
vector<edge> bridges;
int time;

void dfs(int v, int last_id = -1) {
    used[v] = true;
    time++;
    enter[v] = ret[v] = time;
    for (size_t i = 0; i < g[v].size(); i++)
    {
        edge to = g[v][i];
        if (used[to.to] && last_id != to.id)
        {
            ret[v] = min(ret[v], enter[to.to]);
        }
        if (!used[to.to])
        {
            dfs(to.to, to.id);
            ret[v] = min(ret[v], ret[to.to]);
            if (ret[to.to] > enter[v])
                bridges.push_back(to);
        }
    }
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& v)
{
	out << "[";
	for (int i = 0; i < (int)v.size() - 1; i++)
		out << v[i] << ", ";
	return out << v.back() << "]";
}

int main()
{
	ifstream in("bridges.in");
	ofstream out("bridges.out");
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
	sort(bridges.begin(), bridges.end());
	out << bridges.size() << "\n";
	for (size_t i = 0; i < bridges.size(); i++)
		out << bridges[i].id << " ";
	out << "\n";
}
