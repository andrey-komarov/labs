#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <set>

using namespace std;

const long long INF = ((long long)1000000000) * 1000000;

struct graph
{
	struct edge 
	{
		edge(int from, int to, long long p, long long c, long long f, bool ret = false): from(from), to(to), p(p), c(c), f(f), ret(ret) {} 

		int from, to;
		long long p, c, f;
		edge* rev;
		bool ret;

		long long remain()
		{	return c - f;}
	};

	friend ostream& operator<<(ostream&, edge);

	graph(int n) : n(n), g(n) {}

	void add_edge(int from, int to, int c, int p)
	{
		edge* e1 = new edge(from, to, p, c, 0);
		edge* e2 = new edge(to, from, -p, c, c, true);
		e1->rev = e2;
		e2->rev = e1;
		g[from].push_back(e1);
		g[to].push_back(e2);
	}
	
	long long min_cost(int s, int t)
	{
		vector<edge*> path;
		long long ans = 0;
		while (!(path = get_path(s, t)).empty())
		{
			long long mn = numeric_limits<long long>::max();
			long long cost = 0;
			for (vector<edge*>::const_iterator it = path.begin(); it != path.end(); it++)
			{
				mn = min(mn, (*it)->remain());
				cost += (*it)->p;
//				cerr << **it << " ";
			}
//			cerr << "\n";
			ans += mn * cost;
			for (vector<edge*>::iterator it = path.begin(); it != path.end(); it++)
			{
				(*it)->f += mn;
				(*it)->rev->f -= mn;
			}
		}
		return ans;
	}

	pair<long long, vector<pair<int, int> > > assignment(int s, int t)
	{
		long long cost = min_cost(s, t);
		vector<pair<int, int> > ans;
		for (int i = 0; i < n; i++)
		{
			if (i == s|| i == t)
				continue;
			for (vector<edge*>::const_iterator it = g[i].begin(); it != g[i].end(); it++)
			{
				edge *e = *it;
				if (e->to == s || e->to == t || e->remain() > 0 || e->ret)
					continue;
				ans.push_back(make_pair(e->from, e->to));
			}
		}
		return make_pair(cost, ans);
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
			pair<long long, int> now2 = q.top();
			int now = now2.second;
			if (now == t)
				break;
			q.pop();
			if (d[now] < now2.first)
				continue;
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

struct graph2
{
	graph2(int n, int m) : n(n), m(m), g(n)
	{}

	void add_edge(int a, int b)
	{	g[a].insert(b);}
	
	vector<int> matching()
	{
		p = vector<int>(m, -1);
		for (int i = 0; i < n; i++)
		{	
			used = vector<bool>(n, false);
			assert(dfs(i)); // вешайтесь, любители ФП!
		}	
		vector<int> ans(n, -1);
		for (int i = 0; i < m; i++)
		{
			assert(p[i] != -1);
			ans[p[i]] = i;
		}
		return ans;
	}

	void clear(const vector<int>& a)
	{
		assert (a.size() == n);
		for (int i = 0; i < n; i++)
			g[i].erase(a[i]);
	}

	int n, m;
	vector<set<int> > g;
	vector<bool> used;
	vector<int> p;
private:
	bool dfs(int v)
	{
		used[v] = true;
		for (set<int>::iterator it = g[v].begin(); it != g[v].end(); it++)
		{
			int to = *it;
			if (p[to] == -1 || (!used[p[to]] && dfs(p[to])))
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
	ifstream in("multiassignment.in");
	ofstream out("multiassignment.out");
	int n, k;
	in >> n >> k;
	graph g(2 * n + 2);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int a;
			in >> a;
			g.add_edge(i, j + n, 1, a);
		}
		g.add_edge(2 * n, i, k, 0);
		g.add_edge(i + n, 2 * n + 1, k, 0);
	}
	pair<long long, vector<pair<int, int> > > ans = g.assignment(2 * n, 2* n + 1);
	int cnt = ans.second.size() / n;
	out << ans.first << "\n";
	graph2 g2(n,n);
	for (vector<pair<int, int> >::const_iterator it = ans.second.begin(); it != ans.second.end(); it++)
	{
		pair<int, int> e = *it;
		g2.add_edge(e.first, e.second - n);
	}
	for (int i = 0; i < cnt; i++)
	{
		vector<int> matching = g2.matching();
		g2.clear(matching);
		for (vector<int>::const_iterator it = matching.begin(); it != matching.end(); it++)
			out << (*it + 1) << " ";
		out << "\n";
	}
}
