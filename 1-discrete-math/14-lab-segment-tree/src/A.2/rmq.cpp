#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

template <typename T, T (*f)(T, T), T zero>
struct rangetree
{
	rangetree(size_t);
	rangetree(T*, size_t);
	~rangetree();
private:
	struct proxy;
public:
	T get(size_t, size_t) const;
	T operator[](size_t) const;
	proxy operator[](size_t);
	void set(size_t, const T&);
	
private:
	T* a_;
	size_t n_;
	T get(size_t, size_t, size_t, size_t, size_t) const;
	
	friend T& proxy::operator=(const T&);
};

template <typename T, T (*f)(T, T), T zero>
struct rangetree<T, f, zero>::proxy 
{
	size_t pos_;
	rangetree<T, f, zero>* t_;
	proxy(size_t pos, rangetree<T, f, zero>* t): pos_(pos), t_(t){}
	T& operator=(const T&);	
};

int min2(int a, int b)
{
	return a < b ? a : b;
}

int main() {
	ifstream in("rmq.in");
	ofstream out("rmq.out");
	int n;
	in >> n;
	int* a = new int[n];
	for (int i = 0; i < n; i++)
		in >> a[i];
	rangetree<int, min2, 1<<30> t(a, n);
	delete[] a;
	string type;
	int x, y;
	while (in >> type >> x >> y) 
	{
		if (type == "set")
			t[x - 1] = y;
		else 
			out << t.get(x - 1, y) << "\n";
	}
}

template <typename T, T (*f)(T, T), T zero>
T& rangetree<T, f, zero>::proxy::operator=(const T& val)
{
	t_->a_[pos_ + t_->n_] = val;
	for (size_t i = (pos_ + t_->n_) / 2; i > 0; i /= 2)
	{
		t_->a_[i] = f(t_->a_[2 * i], t_->a_[2 * i + 1]);
	}
	return t_->a_[pos_ + t_->n_];
}

template <typename T, T (*f)(T, T), T zero>
rangetree<T, f, zero>::rangetree(size_t m)
{
	for (n_ = 1; n_ < m; n_ *= 2);
	a_ = new T[2 * n_];
	for (size_t i = 0; i < 2 * n_; i++)
		a_[i] = zero;
}

template <typename T, T (*f)(T, T), T zero>
rangetree<T, f, zero>::rangetree(T* b, size_t m)
{
	for (n_ = 1; n_ < m; n_ *= 2);
	a_ = new T[2 * n_];
	for (size_t i = n_; i < n_ + m; i++)
		a_[i] = b[i - n_];
	for (size_t i = n_ + m; i < 2 * n_; i++)
		a_[i] = zero;
	for (size_t i = n_ - 1; i >= 1; i--)
		a_[i] = f(a_[2 * i], a_[2 * i + 1]);
}

template <typename T, T (*f)(T, T), T zero>
rangetree<T, f, zero>::~rangetree()
{
	delete[] a_;
}

template <typename T, T (*f)(T, T), T zero>
T rangetree<T, f, zero>::get(size_t l, size_t r) const
{
	return get(1, 0, n_, l, r);
}

template <typename T, T (*f)(T, T), T zero>
T rangetree<T, f, zero>::get(size_t v, size_t lb, size_t rb, size_t l, size_t r) const
{
	if (r <= lb || rb <= l)
		return zero;
	if (l <= lb && rb <= r)
		return a_[v];
	return f(
		get(2 * v, lb, (lb + rb) / 2, l, r),
		get(2 * v + 1, (lb + rb) / 2, rb, l, r)
	);
}

template <typename T, T (*f)(T, T), T zero>
T rangetree<T, f, zero>::operator[](size_t pos) const
{
	return a_[pos + n_];
}

template <typename T, T (*f)(T, T), T zero>
typename rangetree<T, f, zero>::proxy rangetree<T, f, zero>::operator[](size_t pos)
{
	return proxy(pos, this);
}
