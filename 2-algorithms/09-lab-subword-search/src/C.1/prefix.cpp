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
		int q = p[i - 1];
		while (q > 0 && s[i] != s[q])
			q = p[q - 1];
		p[i] = q;
		if (s[q] == s[i])
			p[i]++;
	}
	for (int i = 0; i < n; i++)
		out << p[i] << " ";
	out << endl;
}
