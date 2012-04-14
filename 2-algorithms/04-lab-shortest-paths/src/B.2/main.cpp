#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
	ifstream in("pathmgep.in");
	ofstream out("pathmgep.out");
	size_t n, s, f;
	in >> n >> s >> f;
	s--;
	f--;
	int g[n][n];
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < n; j++)
			in >> g[i][j];
	int d[n];
	bool used[n];
	const int INF = 1 << 29;
	for (size_t i = 0; i < n; i++)
	{
		d[i] = INF;
		used[i] = false;
	}
	d[s] = 0;
	while (true)	
	{
		size_t minpos = 0;
		int min = INF;
		for (size_t i = 0; i < n; i++)
			if (!used[i] && d[i] < min)
			{
				minpos = i;
				min = d[i];
			}
		if (min == INF)
			break;
		for (size_t i = 0; i < n; i++)
			if (g[minpos][i] > 0 && d[i] > d[minpos] + g[minpos][i])
			{
				d[i] = d[minpos] + g[minpos][i];
			}
		used[minpos] = true;
	}
	if (d[f] == INF)
		out << "-1\n";
	else
		out << d[f] << "\n";
}
