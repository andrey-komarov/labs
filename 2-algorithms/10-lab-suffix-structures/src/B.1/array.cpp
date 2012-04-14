#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

static const size_t ALPHA = 256;

vector<size_t> suffix_array(string s)
{
	s = s + " ";
	size_t n = s.size();
	size_t l = 1;

	vector<size_t> type_(n, 0);

	vector<size_t> count(ALPHA, 0);
	for (size_t i = 0; i < n; i++)
		count[s[i]]++;
	vector<size_t> ptr(ALPHA, 0);
	for (size_t i = 1; i < ALPHA; i++)
		ptr[i] = ptr[i - 1] + count[i - 1];
	for (size_t i = 0; i < n; i++)
		type_[i] = ptr[s[i]];

	count = vector<size_t>(n, 0);
	vector<size_t> pos(n);
	vector<size_t> pre_order(n, 0);
	vector<size_t> order(n, 0);
	vector<size_t> new_type(n, 0);

	while (l <= n)
	{
	    fill(count.begin(), count.end(), 0);
	    for (size_t i = 0; i < n; i++)
		count[type_[i]]++;
	    pos[0] = 0;
	    for (size_t type = 1; type < n; type++)
		pos[type] = pos[type - 1] + count[type - 1];
	    vector<size_t> pos2 = pos;

	    for (size_t i = 0; i < n; i++)
		pre_order[pos[type_[(i + l) % n]]++] = i;

	    for (size_t i = 0; i < n; i++)
		order[pos2[type_[pre_order[i]]]++] = pre_order[i];

	    size_t type_size = 0;

	    for (size_t i = 0; i < n; i++)
	    {
		size_t j = i;
		while (j < n && (type_[order[i]] == type_[order[j]]) && (type_[(order[i] + l) % n] == type_[(order[j] + l) % n]) )
		{
		    new_type[order[j]] = type_size;
		    j++;
		}
		i = j - 1;
		type_size++;
	    }
	    type_ = new_type;
	    l *= 2;
	}

        vector<size_t> inv(n);
        for (size_t i = 0; i < n; i++)
            inv[type_[i]] = i;

        vector<size_t> ans(n - 1);
        for (size_t i = 0; i < n - 1; i++)
            ans[i] = inv[i + 1] + 1;

        return ans;
}

int main()
{
	ifstream in("array.in");
	ofstream out("array.out");
	string s;
	in >> s;
	vector<size_t> array = suffix_array(s);
	for (vector<size_t>::const_iterator it = array.begin(); it != array.end(); it++)
		out << *it << " ";
}
