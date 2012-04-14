#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	ifstream in("prefix.in");
	ofstream out("prefix.out");
	string s;
	in >> s;
	size_t n(s.size());
	size_t p[n];
	p[0] = 0;
	for (size_t i = 1; i < n; i++)
	{
		for (p[i] = p[i - 1]; p[i] > 0 && s[i] != s[p[i]]; p[i] = p[p[i] - 1]);
		if (s[p[i]] == s[i])
			p[i]++;
	}
	for (int i = 0; i < n; i++)
		out << p[i] << " ";
	out << endl;
}
