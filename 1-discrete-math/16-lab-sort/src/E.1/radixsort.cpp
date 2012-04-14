#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void sort(vector<string*>& a, size_t index)
{
	size_t n = a.size();
	size_t cnt[26], pos[26];
	for (size_t i = 0; i < 26; i++)
		cnt[i] = pos[i] = 0;
	for (size_t i = 0; i < n; i++)
	{
		++cnt[(*(a[i]))[index] - 'a'];
	}
	for (size_t i = 1; i < 26; i++)
	{
		pos[i] = pos[i - 1] + cnt[i - 1];
	}
	vector<string*> result(n);
	for (size_t i = 0; i < n; i++)
	{		
		result[
			pos[(*a[i])[index] - 'a']++
		] = a[i];
	}
	a = result;
}

int main()
{
	ifstream in("radixsort.in");
	ofstream out("radixsort.out");
	size_t n, l, k;
	in >> n >> l >> k;
	vector<string*> a(n);
	for (size_t i = 0; i < n; i++)
	{
		string s;
		in >> s;
		a[i] = new string(s);
	}
	for (size_t phase = 0; phase < k; phase++)
		sort(a, l - phase - 1);
	for (size_t i = 0; i < n; i++)
		out << *a[i] << "\n";
}
