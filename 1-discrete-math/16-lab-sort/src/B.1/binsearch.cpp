#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int lower_bound(const vector<int>& a, int elem)
{
	int l = -1;
	int r = a.size() - 1;
	while (l + 1 < r)
	{
		int c = (l + r) / 2;
		if (a[c] < elem)
			l = c;
		else
			r = c;
	}
	return a[l + 1] == elem ? (l + 1) : -2;
}

int upper_bound(const vector<int>& a, int elem)
{
	int l = 0;
	int r = a.size();
	while (l + 1 < r)
	{
		int c = (l + r) / 2;
		if (a[c] <= elem)
			l = c;
		else
			r = c;
	}
	return a[r - 1] == elem ? (r - 1) : -2;
}



int main()
{
	ifstream in("binsearch.in");
	ofstream out("binsearch.out");
	size_t n;
	in >> n;
	vector<int> a(n);
	for (size_t i = 0; i < n; i++)
		in >> a[i];
	size_t m;
	in >> m;
	for (size_t i = 0; i < m; i++)
	{
		int b;
		in >> b;
		out << lower_bound(a, b) + 1 << " ";
		out << upper_bound(a, b) + 1<< "\n";
	}
}
