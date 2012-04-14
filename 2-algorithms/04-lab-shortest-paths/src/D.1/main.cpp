#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

int main()
{
	ifstream in("pathbgep.in");
	ofstream out("pathbgep.out");
	size_t n, m;
	in >> n >> m;
	vector<pair<size_t, int> > g[n];
	for (size_t i = 0; i < m; i++)
	{
		size_t a, b, w;
		in >> a >> b >> w;
		a--;
		b--;
		g[a].push_back(make_pair(b, w));
		g[b].push_back(make_pair(a, w));
	}
	const int INF = 1 << 29;
	int d[n];
	bool used[n];
	for (size_t i = 0; i < n; i++)
	{
		d[i] = INF;
		used[i] = false;
	}
	d[0] = 0;
	priority_queue<pair<int, int> > q;
	q.push(make_pair(0, 0));
	while (!q.empty())
	{
		size_t now = q.top().second;
		q.pop();
		if (used[now])
			continue;
		used[now] = true;
		for (size_t i = 0; i < g[now].size(); i++)
		{
			size_t to = g[now][i].first;
			if (d[to] > d[now] + g[now][i].second)
			{
				d[to] = d[now] + g[now][i].second;
				q.push(make_pair(-d[to], to));
			}
		}
	}
	for (size_t i = 0; i < n; i++)
		out << d[i] << " ";
	out << "\n";
}
