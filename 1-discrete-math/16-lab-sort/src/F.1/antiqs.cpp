#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{
	ifstream in("antiqs.in");
	ofstream out("antiqs.out");
	int n;
	in >> n;
	switch (n)
	{
		case 2: out << "1 2"; return 0;
		case 3: out << "1 3 2"; return 0;
	}	
	vector<int> a(n);
	a[0] = 1;
	a[1] = 3;
	a[2] = 2;
	for (int i = 3; i < n; i++)
	{
		a[i] = a[i / 2];
		a[i / 2] = i + 1;
	}
	for (int i = 0; i < n; i++)
		out << a[i] << " ";
}

