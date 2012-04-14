#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

vector<int>* g;
size_t* d;
bool* used;

int main()
{
	ifstream in("pathbge1.in");
	ofstream out("pathbge1.out");
	size_t n, m;
	in >> n >> m;
	g = new vector<int>[n];
	for (size_t i = 0; i < m; i++)
	{
		int a, b;
		in >> a >> b;
		a--;
		b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	d = new size_t[n];
	used = new bool[n];
	for (size_t i = 0; i < n; i++)
	{
		d[i] = n;
		used[i] = false;
	}
	d[0] = 0;
	queue<int> q;
	used[0] = true;
	q.push(0);
	while (!q.empty())
	{
		int now = q.front();
		q.pop();
		for (size_t i = 0; i < g[now].size(); i++)
		{
			int to = g[now][i];
			if (!used[to])
			{
				used[to] = true;
				d[to] = d[now] + 1;
				q.push(to);
			}
		}
	}
	for (size_t i = 0; i < n; i++)
		out << d[i] << " ";
	out << "\n";
}
