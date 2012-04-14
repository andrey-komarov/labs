#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <set>

using namespace std;

int main()
{
	ifstream in("negcycle.in");
	ofstream out("negcycle.out");
	const long long INF = 1L << 50;
	size_t n;
	in >> n;
	long long a[n + 1][n + 1];
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++)
			in >> a[i][j];
	long long d[n];
	int p[n];

	for (size_t i = 0; i < n; i++)
	{
		d[i] = INF;
		p[i] = -1;
	}
	d[0] = 0;
	int lastmodify = -1;
	int lastmodifypos = -1;
	for (size_t k = 1; k < n + 1; k++)
	{
		for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < n; j++)
				if (d[j] > d[i] + a[i][j])
				{
					p[j] = i;
					d[j] = d[i] + a[i][j];
					lastmodify = k;
					lastmodifypos = j;
				}
	}
	if (lastmodify != n)
	{
		out << "NO\n";
		return 0;
	}
	bool found = false;
	size_t start = lastmodifypos;
	vector<int> ans;
	set<int> used;
	size_t now = start;
	int cycle = -1;
        while (true)
	{
		ans.push_back(now);
		if (used.count(now) != 0)
		{
			cycle = now;
			break;
		}
		used.insert(now);
		now = p[now];
	}
	cerr << cycle << "\n";
	int first = -1, second = -1;
	for (size_t i = 0; i < ans.size(); i++)
	{
		if (first == -1 && ans[i] == cycle)
			first = i;
		else if (first != -1 && ans[i] == cycle)
			second = i;
	}
	cerr << first << " " << second << "\n";
	out << "YES\n" << (second - first + 1) << "\n";
	for (int i = second; i >= first; i--)
		out << (ans[i] + 1) << " ";
	out << "\n";
}
