#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	ifstream in("z.in");
	ofstream out("z.out");
	string s;
	in >> s;
	size_t n = s.size();
	int z[n];
	z[0] = -1;
	int L = 0;
	int R = 1;
	for (int i = 1; i < n; i++)
	{
		z[i] = max(0, min(z[i - L], R - i));
		while (z[i] + i < n && s[z[i]] == s[i + z[i]])
			z[i]++;
		if (i + z[i] > R)
		{
			R = i + z[i];
			L = i;
		}
	}
	for (int i = 1; i < n; i++)
		out << z[i] << " ";
	out << endl;
}

