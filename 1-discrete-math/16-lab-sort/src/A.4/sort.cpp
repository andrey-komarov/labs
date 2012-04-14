#include <iostream>
#include <vector>
#include <fstream>
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

vector<int> operator+(const vector<int>& a, const int b)
{
	vector<int> ans(a);
	ans.push_back(b);
	return ans;
}

vector<int> operator+(const int a, const vector<int>& b)
{
	vector<int> ans;
	ans.push_back(a);
	for (size_t i = 0; i < b.size(); i++)
		ans.push_back(b[i]);
	return ans;
}

vector<int> operator+(const vector<int>& a, const vector<int>& b)
{
	vector<int> ans(a);
	for (size_t i = 0; i < b.size(); i++)
		ans.push_back(b[i]);
	return ans;
}

bool les(const int& a, const int& b)
{
	return a < b;
}

bool gre(const int& a, const int& b)
{
	return a > b;
}

struct bind
{
	int second_;
	bool (*f_)(const int&, const int&);
	
	bind(bool (*f)(const int&, const int&), const int& a): second_(a)
	{
		f_ = f;
		second_ = a;
	}
	
	bool operator()(const int& a) {
		return f_(a, second_);
	}
};

vector<int> select(bind pred, const vector<int>& a)
{
	vector<int> ans;
	for (size_t i = 0; i < a.size(); i++)
		if (pred(a[i]))
			ans.push_back(a[i]);
	return ans;
}

int count(const vector<int>& a, int c)
{
	int cnt = 0;
	for (size_t i = 0; i < a.size(); i++)
		cnt += (a[i] == c) ? 1 : 0;
	return cnt;
}

vector<int> sort(vector<int> a)
{
	if (a.size() < 2)
		return a;
	int mid = a[abs(rand()) % a.size()];
	return 
		sort(select(bind(les, mid), a)) +
		vector<int>(count(a, mid), mid) + 
		sort(select(bind(gre, mid), a));
}

int main()
{
	ifstream in("sort.in");
	ofstream out("sort.out");
	int n;
	in >> n;
	vector<int> a(n);
	for (int i = 0; i < n; i++)
		in >> a[i];
	a = sort(a);
	for (int i = 0; i < n; i++)
		out << a[i] << " ";
}
