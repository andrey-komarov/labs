#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>

using namespace std;

template <typename T, T (*f)(T, T)>
struct rangetree
{
	rangetree(size_t, T);
	rangetree(T*, size_t, T);
	~rangetree();
private:
	struct proxy;
public:
	T get(size_t, size_t) const;
	T operator[](size_t) const;
	proxy operator[](size_t);
	void set(size_t, const T&);
	
private:
	T zero;
	T* a_;
	size_t n_;
	T get(size_t, size_t, size_t, size_t, size_t) const;
	
	friend T& proxy::operator=(const T&);
};

template <typename T, T (*f)(T, T)>
struct rangetree<T, f>::proxy 
{
	size_t pos_;
	rangetree<T, f>* t_;
	proxy(size_t pos, rangetree<T, f>* t): pos_(pos), t_(t){}
	T& operator=(const T&);	
};


int modulo;
struct matrix
{
	int a_[2][2];
	//~ static int modulo;
	
	matrix(){}
	
	matrix(int a, int b, int c, int d)
	{
		a_[0][0] = a % modulo;
		a_[0][1] = b % modulo; 
		a_[1][0] = c % modulo;
		a_[1][1] = d % modulo;
	}
	
	friend ostream& operator<<(ostream&, const matrix&);
};

matrix mult(matrix m1, matrix m2)
{
	return matrix(
		m1.a_[0][0] * m2.a_[0][0] + m1.a_[0][1] * m2.a_[1][0],
		m1.a_[0][0] * m2.a_[0][1] + m1.a_[0][1] * m2.a_[1][1],
		m1.a_[1][0] * m2.a_[0][0] + m1.a_[1][1] * m2.a_[1][0],
		m1.a_[1][0] * m2.a_[0][1] + m1.a_[1][1] * m2.a_[1][1]
	);
}
	
int main() {
	ifstream in("crypto.in");
	ofstream out("crypto.out");
	int n, r, m;
	in >> r >> n >> m;
	modulo = r;
	matrix* a = new matrix[n];
	for (int i = 0; i < n; i++)
	{
		int a1, a2, a3, a4;
		in >> a1 >> a2 >> a3 >> a4;
		a[i] = matrix(a1, a2, a3, a4);
	}

	matrix one(1L, 0L, 0L, 1L);
	rangetree<matrix, mult> t(a, n, one);

	for (int i = 0; i < m; i++) 
	{
		int l, r;
		in >> l >> r;
		out << t.get(l - 1, r) << "\n";
	}
}

ostream& operator<<(ostream& out, const matrix& m)
{
	return out << m.a_[0][0] << " " << m.a_[0][1] << "\n" << m.a_[1][0] << " " << m.a_[1][1] << "\n";
}

template <typename T, T (*f)(T, T)>
T& rangetree<T, f>::proxy::operator=(const T& val)
{
	t_->a_[pos_ + t_->n_] = val;
	for (size_t i = (pos_ + t_->n_) / 2; i > 0; i /= 2)
	{
		t_->a_[i] = f(t_->a_[2 * i], t_->a_[2 * i + 1]);
	}
	return t_->a_[pos_ + t_->n_];
}

template <typename T, T (*f)(T, T)>
rangetree<T, f>::rangetree(size_t m, T z): zero(z)
{
	for (n_ = 1; n_ < m; n_ *= 2);
	a_ = new T[2 * n_];
	for (size_t i = 0; i < 2 * n_; i++)
		a_[i] = zero;
}

template <typename T, T (*f)(T, T)>
rangetree<T, f>::rangetree(T* b, size_t m, T z): zero(z)
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

template <typename T, T (*f)(T, T)>
rangetree<T, f>::~rangetree()
{
	delete[] a_;
}

template <typename T, T (*f)(T, T)>
T rangetree<T, f>::get(size_t l, size_t r) const
{
	return get(1, 0, n_, l, r);
}

template <typename T, T (*f)(T, T)>
T rangetree<T, f>::get(size_t v, size_t lb, size_t rb, size_t l, size_t r) const
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

template <typename T, T (*f)(T, T)>
T rangetree<T, f>::operator[](size_t pos) const
{
	return a_[pos + n_];
}

template <typename T, T (*f)(T, T)>
typename rangetree<T, f>::proxy rangetree<T, f>::operator[](size_t pos)
{
	return proxy(pos, this);
}

