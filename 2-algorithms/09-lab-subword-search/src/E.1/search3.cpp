#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> z(string s)
{
        size_t n = s.size();
	vector<int> z(n);
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
	return z;
}

string reverse(string s)
{
	reverse(s.begin(), s.end());
	return s;
}

int main()
{
	ifstream in("search3.in");
	ofstream out("search3.out");
	string p, t;
	in >> p >> t;
	string pt = p + "#" + t;
	string rprt = reverse(p) + "#" + reverse(t);
	vector<int> zpt = z(pt);
	vector<int> zrprt = z(rprt);
	int n = t.size();
	int m = p.size();
	vector<int> ans;
	for (int i = 0; i < n - m + 1; i++)
	{
		if (zpt[m + i + 1] + zrprt[n - i + 1] >= m - 1)
			ans.push_back(i + 1);
	}
	out << ans.size() << endl;
	for (size_t i = 0; i < ans.size(); i++)
		out << ans[i] << " ";
	out << endl;
}

