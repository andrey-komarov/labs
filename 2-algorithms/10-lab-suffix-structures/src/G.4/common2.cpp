#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

typedef long long hash_t;
typedef vector<hash_t> hashes;

static const hash_t X = 239017;

string s1, s2;

hashes hash(const string& s, size_t len, bool need_sort = false)
{
	hash_t XPOW = 1;
	for (size_t i = 0; i < len - 1; i++)
		XPOW *= X;

	hash_t hash = 0;
	for (size_t i = 0; i < len; i++)
		hash = hash * X + s[i];

	hashes m;
	m.reserve(s.size() - len + 1);
	for (size_t i = 0; i < s.size() - len + 1; i++)
	{
		m.push_back(hash);
		if (i != s.size() - len)
			hash = (hash - XPOW * s[i]) * X + s[i + len];
	}
	if (need_sort)
	{
		sort(m.begin(), m.end());
		m.resize(unique(m.begin(), m.end()) - m.begin());
	}
	return m;
}

bool check(size_t len)
{
	hashes m1 = hash(s1, len);
	hashes m2 = hash(s2, len, true);
	for (hashes::const_iterator it = m1.begin(); it != m1.end(); it++)
		if (binary_search(m2.begin(), m2.end(), *it))
			return true;
	return false;
}

void update(string& s, const string& from, size_t pos, size_t len)
{
	if (s == "")
	{
		s = from.substr(pos, len);
		return;
	}
	for (size_t i = 0; i < len; i++)
		if (from[pos + i] < s[i])
		{
			copy(from.begin() + pos + i, from.begin() + pos + len, s.begin() + i);
			return;
		}
		else if (from[pos + i] > s[i])
			return;
}

string common(size_t len)
{
	hashes m1 = hash(s1, len);
	hashes m2 = hash(s2, len, true);
	string ans = "";
	hashes common(0); 
	for (hashes::const_iterator it = m1.begin(); it != m1.end(); it++)
		if (binary_search(m2.begin(), m2.end(), *it))
			common.push_back(*it);
	sort(common.begin(), common.end());

	hash_t XPOW = 1;
	for (size_t i = 0; i < len - 1; i++)
		XPOW *= X;

        hash_t hash = 0;
	for (size_t i = 0; i < len; i++)
		hash = hash * X + s1[i];
	for (size_t i = 0; i < s1.size() - len + 1; i++)
	{
		if (binary_search(common.begin(), common.end(), hash))
			update(ans, s1, i, len);
		if (i != s1.size() - len)
			hash = (hash - XPOW * s1[i]) * X + s1[i + len];
	}


	return ans;
}

int main()
{
	ifstream in("common.in");
	ofstream out("common.out");
	in >> s1 >> s2;
	size_t L = 0;
	size_t R = min(s1.size(), s2.size()) + 1;
	while (R - L > 1)
	{
		size_t c = (L + R) / 2;
		if (check(c))
			L = c;
		else
			R = c;
	}
	out << common(L) << "\n" ;
}
