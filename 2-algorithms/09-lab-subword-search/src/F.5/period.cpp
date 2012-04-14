#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	ifstream in("period.in");
	ofstream out("period.out");
	string s;
	in >> s;
	size_t n(s.size());
	size_t* p = new size_t[n];
	p[0] = 0;
	for (size_t i = 1; i < n; i++)
	{
		for (p[i] = p[i - 1]; p[i] > 0 && s[i] != s[p[i]]; p[i] = p[p[i] - 1]);
		if (s[p[i]] == s[i])
			p[i]++;
	}
	int period = n - p[n - 1];
	if (n % period != 0)
		out << n << endl;
	else 
		out << period << endl; 
}
