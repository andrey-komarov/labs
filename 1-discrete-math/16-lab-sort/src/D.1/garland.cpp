#include <fstream>
#include <vector>
#include <iostream>
#include <cstdio>

using namespace std;

int n;
double A;
vector<double> h;

bool check(double c)
{
	h[0] = A;
	h[1] = c;
	bool fl = true;
	for (int i = 2; i < n; i++)
	{		
		h[i] = 2 * h[i - 1] - h[i - 2] + 2;
		if (h[i] < 0)
			fl = false;
	}
	return fl;
}

int main()
{
	ifstream in("garland.in");
	freopen("garland.out", "w", stdout);
	in >> n >> A;
	h = vector<double>(n);
	double L = 0, R = A;
	for (int t = 0; t < 100; t++)
	{
		double C = (L + R) * .5;
		if (check(C))
			R = C;
		else
			L = C;
	}
	printf("%.2f\n", h[n - 1]);
}

