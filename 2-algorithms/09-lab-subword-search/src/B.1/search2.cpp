#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

static const long long X = 997;

int main()
{
	ifstream in("search2.in");
	ofstream out("search2.out");
	string p, t;
	in >> p >> t;
	size_t n(t.size());
	size_t m(p.size());
	if (m > n)
	{
		out << "0" << endl;
		return 0;	
	}
	long long hash = 0;
	long long hasht = 0;
	long long XPOW = 1;
	for (int i = 0; i < m; i++)
	{
		XPOW *= i == 0 ? 1 : X;
		hash = hash * X + p[i];
		hasht = hasht * X + t[i];
	}
	vector<int> ans;
	for (int i = 0; i < n - m + 1; i++)
	{
		if (hasht == hash)
			ans.push_back(i + 1);
		if (i != n - m)
			hasht = (hasht - XPOW * t[i]) * X + t[i + m];
	}
	out << ans.size() << endl;
	for (vector<int>::iterator it = ans.begin(); it != ans.end(); it++)
		out << *it << " ";
	out << "\n";
}
