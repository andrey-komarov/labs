#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

typedef long long T;
const T zero = 2000000000000000000L;

struct rangetree
{
	rangetree(size_t);
	rangetree(T*, size_t);
	~rangetree();
	T get(size_t, size_t);
	void set(size_t, size_t, const T&);
	void add(size_t, size_t, const T&);
	
private:
	T *min_, *set_, *add_;
	bool* isset_;
	size_t n_;
	void update(size_t);
	T get(size_t, size_t, size_t, size_t, size_t);
	void set(size_t, size_t, size_t, size_t, size_t, const T&);
	void add(size_t, size_t, size_t, size_t, size_t, const T&);
};

int main() {
	ifstream in("rmq2.in");
	ofstream out("rmq2.out");
	int n;
	in >> n;
	T* a = new T[n];
	for (int i = 0; i < n; i++)
		in >> a[i];
	rangetree t(a, n);
	delete[] a;
	string type;
	int l, r;
	T val;
	while (in >> type >> l >> r) 
	{
		if (type == "add")
		{
			in >> val;
			t.add(l - 1, r, val);
		}
		else if (type == "min")
		{
			out << t.get(l - 1, r) << "\n";
		} 
		else 
		{
			in >> val;
			t.set(l - 1, r, val);
		}
		
	}
}

rangetree::rangetree(size_t m)
{
	for (n_ = 1; n_ < m; n_ *= 2);
	min_ = new T[2 * n_];
	set_ = new T[2 * n_];
	add_ = new T[2 * n_];
	isset_ = new bool[2 * n_];
	for (size_t i = 0; i < 2 * n_; i++)
	{
		min_[i] = zero;
		add_[i] = 0;
		isset_[i] = false;
	}
}

rangetree::rangetree(T* b, size_t m)
{
	for (n_ = 1; n_ < m; n_ *= 2);
	min_ = new T[2 * n_];
	set_ = new T[2 * n_];
	add_ = new T[2 * n_];
	isset_ = new bool[2 * n_];
	for (size_t i = n_; i < n_ + m; i++)
		min_[i] = b[i - n_];
	for (size_t i = n_ + m; i < 2 * n_; i++)
		min_[i] = zero;
	for (size_t i = n_ - 1; i >= 1; i--)
		min_[i] = min(min_[2 * i], min_[2 * i + 1]);
	for (size_t i = 0; i < 2 * n_; i++)
	{
		add_[i] = 0;
		isset_[i] = false;
	}
}

rangetree::~rangetree()
{
	delete[] min_;
	delete[] add_;
	delete[] isset_;
	delete[] set_;
}

T rangetree::get(size_t l, size_t r)
{
	return get(1, 0, n_, l, r);
}

T rangetree::get(size_t v, size_t lb, size_t rb, size_t l, size_t r)
{
	update(v);
	if (r <= lb || rb <= l)
		return zero;
	if (l <= lb && rb <= r)
		return min_[v];
	return min(
		get(2 * v, lb, (lb + rb) / 2, l, r),
		get(2 * v + 1, (lb + rb) / 2, rb, l, r)
	);
}

void rangetree::set(size_t l, size_t r, const T& val)
{
	set(1, 0, n_, l, r, val);
}

void rangetree::set(size_t v, size_t lb, size_t rb, size_t l, size_t r, const T& val)
{
	update(v);
	if (r <= lb || rb <= l)
		return;
	if (l <= lb && rb <= r)
	{
		set_[v] = val;
		isset_[v] = true;
		min_[v] = val;
		add_[v] = 0;
		return;
	}
	set(2 * v, lb, (lb + rb) / 2, l, r, val);
	set(2 * v + 1, (lb + rb) / 2, rb, l, r, val);
	update(v);
}

void rangetree::add(size_t l, size_t r, const T& val)
{
	add(1, 0, n_, l, r, val);
}

void rangetree::add(size_t v, size_t lb, size_t rb, size_t l, size_t r, const T& val)
{
	update(v);
	if (r <= lb || rb <= l)
		return;
	if (l <= lb && rb <= r)
	{
		min_[v] += val;
		add_[v] = val;
		return;
	}
	add(2 * v, lb, (lb + rb) / 2, l, r, val);
	add(2 * v + 1, (lb + rb) / 2, rb, l, r, val);
	update(v);
}

void rangetree::update(size_t v)
{
	if (v >= n_) 
	{
		add_[v] = 0;
		if (isset_[v])
		{
			isset_[v] = false;
			min_[v] = set_[v];
		}
		return;
	}
	if (add_[v] != 0) 
	{
		if (isset_[2 * v])
			set_[2 * v] += add_[v];
		else
			add_[2 * v] += add_[v];			
		min_[2 * v] += add_[v];
		if (isset_[2 * v + 1])
			set_[2 * v + 1] += add_[v];
		else
			add_[2 * v + 1] += add_[v];
		min_[2 * v + 1] += add_[v];
		add_[v] = 0;		
	}
	if (isset_[v])
	{
		set_[2 * v] = set_[v];
		min_[2 * v] = set_[v];
		isset_[2 * v] = true;
		set_[2 * v + 1] = set_[v];
		min_[2 * v + 1] = set_[v];
		isset_[2 * v + 1] = true;
		add_[2 * v] = 0;
		add_[2 * v + 1] = 0;
		isset_[v] = false;
	}
	min_[v] = min(min_[2 * v], min_[2 * v + 1]);
}
