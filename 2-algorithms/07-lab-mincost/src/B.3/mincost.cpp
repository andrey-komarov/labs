#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <cstdlib>
#include <algorithm>

using namespace std;

const long long INF = ((long long)1000000000) * 1000000;

struct graph
{
	struct edge 
	{
		edge(int from, int to, long long p, long long c, long long f): from(from), to(to), p(p), c(c), f(f) {} 

		int from, to;
		long long p, c, f;
		edge* rev;

		long long remain()
		{	return c - f;}
	};

	friend ostream& operator<<(ostream&, edge);

	graph(int n) : n(n), g(n) {}

	void add_edge(int from, int to, int c, int p)
	{
		edge* e1 = new edge(from, to, p, c, 0);
		edge* e2 = new edge(to, from, -p, c, c);
		e1->rev = e2;
		e2->rev = e1;
		g[from].push_back(e1);
		g[to].push_back(e2);
	}
	
	long long min_cost(int s, int t)
	{
		vector<edge*> path;
		vector<edge*> empty;
		long long ans = 0;
		while ((path = get_path(s, t)) != empty)
		{
			long long mn = numeric_limits<long long>::max();
			long long cost = 0;
			for (vector<edge*>::const_iterator it = path.begin(); it != path.end(); it++)
			{
				mn = min(mn, (*it)->remain());
				cost += (*it)->p;
			}
			ans += mn * cost;
			for (vector<edge*>::iterator it = path.begin(); it != path.end(); it++)
			{
				(*it)->f += mn;
				(*it)->rev->f -= mn;
			}
		}
		return ans;
	}

private:

	int n;
	vector<vector<edge*> > g;

	vector<edge*> get_path(int s, int t)
	{
		vector<long long> d(n, INF);
		d[s] = 0;
		vector<bool> used(n, false);
		vector<edge*> p(n, (edge*)NULL);
		priority_queue<pair<long long, int> > q;
		q.push(make_pair(0, s));
		while (!q.empty())
		{
			int now = q.top().second;
			q.pop();
			used[now] = true;
			for (vector<edge*>::const_iterator it = g[now].begin(); it != g[now].end(); it++)
			{
				edge* e = *it;
				if (d[now] + e->p < d[e->to] && e->remain() > 0) 
				{
					d[e->to] = d[now] + e->p;
					p[e->to] = e;
					q.push(make_pair(-d[e->to], e->to));
				}
			}
		}
		if (d[t] == INF)
			return vector<edge*>();
		vector<edge*> ans;
		edge* now = p[t];
		while (now != NULL)
		{
			ans.push_back(now);
			now = p[now->from];
		}
		return ans;
	}


};

ostream& operator<<(ostream& out, graph::edge e)
{	return out << "[" << e.from << "->" << e.to << ":: cost:" << e.p << ", rem::" << e.remain() << "]";}

int main()
{
	ifstream in("mincost.in");
	ofstream out("mincost.out");
	int n, m;
	in >> n >> m;
	graph g(n);
	for (int i = 0; i < m; i++)
	{
		int a, b, c, p;
		in >> a>> b >> c >> p;
		g.add_edge(a - 1, b - 1, c, p);
	}
	out << g.min_cost(0, n - 1) << "\n";
}
