#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int main()
{
	ifstream in("pathsg.in");
	ofstream out("pathsg.out");
	size_t n, m;
	in >> n >> m;
	int a[n][n];
	const int INF = 1 << 29;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
			a[i][j] = INF;
		a[i][i] = 0;
	}
	for (size_t i = 0; i < m; i++)
	{
		int s, f, w;
		in >> s >> f >> w;
		a[s - 1][f - 1] = min(a[s - 1][f - 1], w);
	}
	for (size_t k = 0; k < n; k++)
		for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < n; j++)
				a[i][j] = min(a[i][j], a[i][k] + a[k][j]);
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n; j++)
			out << a[i][j] << " ";
		out << "\n";
	}
}
