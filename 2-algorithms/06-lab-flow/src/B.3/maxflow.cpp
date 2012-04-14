#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

struct edge
{
	int from, to;
	long long c, f;
	edge* rev;

	edge(int from, int to, long long c, long long f) : 
		from(from), to(to), c(c), f(f), rev(NULL)
	{}

	long long remain() const
	{	return c - f;}
};

ostream& operator<<(ostream& out, const edge& e)
{	return out << "[" << e.from << "->" << e.to << " :: " << e.f << "/" << e.c<< "]";}
                                     
vector<vector<edge*> > g;
vector<int> h;
vector<int> start;
long long min_allowed_w = 1;
int n;

bool bfs()
{
	vector<bool> used(n, false);
	used[0] = true;
	h[0] = 0;
	h[n - 1] = -1;
	queue<int> q;
	q.push(0);
	while (!q.empty())
	{
		int now = q.front();
		q.pop();
		for (vector<edge*>::const_iterator it = g[now].begin(); it != g[now].end(); it++)
		{
			if (!used[(*it)->to] && (*it)->remain() >= min_allowed_w)
			{
				used[(*it)->to] = true;
				q.push((*it)->to);
				h[(*it)->to] = h[now] + 1;
			}
		}
	}
	return h[n - 1] != -1;
}

vector<edge*> path;
int revision = 1;
vector<int> used;

bool dfs(int v)
{
	used[v] = revision;
	if (v == n - 1)
		return true;
	for (; start[v] < (int)g[v].size(); start[v]++)
	{
		edge* to = g[v][start[v]];
		if (to->remain() >= min_allowed_w && used[to->to] != revision)
		{
			path.push_back(to);
			if (dfs(to->to))
				return true;
			path.pop_back();
		}
	}
	return false;
}

long long ans = 0;

void push()
{
	long long mn = numeric_limits<long long>::max();
	for (vector<edge*>::const_iterator it = path.begin(); it != path.end(); it++)
	{
		mn = min(mn, (*it)->remain());
	}
	for (vector<edge*>::iterator it = path.begin(); it != path.end(); it++)
	{
		(*it)->f += mn;
		(*it)->rev->f -= mn;
//		cerr << **it << " ";
	}
//	cerr << "\n";
	ans += mn;
}

int main()
{
	ifstream in("maxflow.in");
	ofstream out("maxflow.out");
	int m;
	in >> n >> m;
	h.resize(n);
	g.resize(n);
	used = vector<int>(n, 0);
	for (int i = 0; i < m; i++)
	{
		int a, b, w;
		in >> a >> b >> w;
		a--;
		b--;
		edge * e1 = new edge(a, b, w, 0);
		edge * e2 = new edge(b, a, w, w);
		e1->rev = e2;
		e2->rev = e1; 
		g[a].push_back(e1);
		g[b].push_back(e2);
	}	
	while (bfs())
	{
		start = vector<int>(n, 0);
		while (dfs(0))
		{
			revision++;
			push();
			path.clear();
		}
		revision++;
	}
	out << ans << "\n";
}
