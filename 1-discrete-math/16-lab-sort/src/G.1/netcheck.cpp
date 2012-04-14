#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

vector<vector<int> > a, b;
vector<int> size;
int n, m, k;

void swap(int& mask, int p1, int p2)
{
	int v1 = (mask & (1 << p1)) >> p1;
	int v2 = (mask & (1 << p2)) >> p2;
	mask &= ~((1 << p1) | (1 << p2));
	mask |= (v1 << p2) | (v2 << p1);
}


void apply(int& mask, int p1_, int p2_)
{
	int p1 = min(p1_, p2_);
	int p2 = max(p1_, p2_);
	int v1 = (mask & (1 << p1)) >> p1;
	int v2 = (mask & (1 << p2)) >> p2;
	if (v1 > v2)
		swap(mask, p1, p2);
}

bool sorted(int mask)
{
	for (int i = 0; i < n - 1; i++)
	{
		int v1 = (mask & (1 << i)) >> i;
		int v2 = (mask & (1 << (i + 1))) >> (i + 1);
		if (v1 > v2)
			return false;
	}
	return true;
}

bool check(int mask)
{
	for (int layer = 0; layer < k; layer++)
		for (int comparator = 0; comparator < size[layer]; comparator++)
			apply(mask, a[layer][comparator], b[layer][comparator]);
	return sorted(mask);
}

int main()
{
	ifstream in("netcheck.in");
	ofstream out("netcheck.out");
	in >> n >> m >> k;
	a.resize(k);
	b.resize(k);
	size.resize(k);
	for (int i = 0; i < k; i++)
	{
		in >> size[i];
		a[i].resize(size[i]);
		b[i].resize(size[i]);
		for (int j = 0; j < size[i]; j++)
		{
			in >> a[i][j] >> b[i][j];
			a[i][j]--;
			b[i][j]--;
		}
	}
	bool fl = true;
	for (int mask = 0; mask < (1 << n); mask++)
	{
		fl &= check(mask);
	}
	out << (fl ? "Yes" : "No");
}
