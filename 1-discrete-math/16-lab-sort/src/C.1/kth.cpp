#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;

ostream& operator<<(ostream& out, const vector<int>& a)
{
	out << "[";
	for (size_t i = 0; i < a.size(); i++)
	{
		out << a[i];
		if (i != a.size() - 1)
			out << ", ";			
	}
	out << "]";
	return out;
}

int nth_element(int* begin, int* end, int pos)
{
	int mid = *(begin + (end - begin) / 2);
	int cnt_less = 0, cnt_equal = 0;
	for (int *i = begin; i != end; i++)
	{
		if (*i == mid)
			cnt_equal++;
		else if (*i < mid)
			cnt_less++;
	}
	if (cnt_less < pos && cnt_less + cnt_equal >= pos)
		return mid;
	if (cnt_less >= pos)
	{
		int *pnt = begin;
		for (int *i = begin; i != end; i++)
			if (*i < mid)
			{
				*pnt = *i;
				pnt++;
			}
		return nth_element(begin, pnt, pos);
	}
	int *pnt = begin;
	for (int *i = begin; i != end; i++)
		if (*i > mid)
		{
			*pnt = *i;
			pnt++;
		}
	return nth_element(begin, pnt, pos - cnt_equal - cnt_less);
}

int main()
{
	ifstream in("kth.in");
	ofstream out("kth.out");
	int n, k, A, B, C;
	in >> n >> k >> A >> B >> C;
	int *a = new int[n];
	in >> a[0] >> a[1];
	for (int i = 2; i < n; i++)
		a[i] = A * a[i - 2] + B * a[i - 1] + C;
	out << nth_element(a, a + n, k) << "\n";
	delete[] a;
}

