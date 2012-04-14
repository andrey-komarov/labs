#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

size_t hash(const int& a)
{
	return 239017 * a;
}

template<typename T, size_t (*f)(const T&) = hash>
struct set
{
	set(size_t capacity);
	set(const set<T, f>&);
	~set();
	void insert(const T&);
	void remove(const T&);
	bool exists(const T&) const;
private:
	const static size_t initial_capacity = 8;
	T *items_;
	bool *notfree_;
	size_t capacity_, size_;
};

int main()
{
	freopen("set.in", "r", stdin);
	freopen("set.out", "w", stdout);
	set<int> a(10000019);
	char type[6];
	int num;
	while (scanf("%s %d", type, &num) != EOF)
	{
		if (type[0] == 'i')
			a.insert(num);
		else if (type[0] == 'd')
			a.remove(num);
		else if (type[0] == 'e')
			printf("%s\n", (a.exists(num) ? "true" : "false"));
	}
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::set(size_t capacity): capacity_(capacity), size_(0)
{
	items_ = new T[capacity_];
	notfree_ = new bool[capacity_];
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::set(const set<T, f>& other): capacity_(other.capacity_), size_(other.size_)
{
	items_ = new T[capacity_];
	for (size_t i = 0; i < capacity_; i++)
	{
		items_[i] = other.items_[i];
	}
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::~set()
{
	delete[] items_;
	delete[] notfree_;
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::insert(const T& item)
{
	if (exists(item))
		return;
	size_t pos = ((f(item) % capacity_) + capacity_) % capacity_;
	while (notfree_[pos])
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	notfree_[pos] = true;
	items_[pos] = item;
	size_++;
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::remove(const T& item)
{
	if (!exists(item))
		return;
	size_t pos = ((f(item) % capacity_) + capacity_) % capacity_;
	while (items_[pos] != item)
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	vector<T> toadd;
	notfree_[pos] = false;
	pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	size_--;
	while (notfree_[pos])
	{
		toadd.push_back(items_[pos]);
		notfree_[pos] = false;
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
		size_--;
	}
	for (size_t i = 0; i < toadd.size(); i++)
		insert(toadd[i]);
}

template<typename T, size_t (*f)(const T&)>
bool set<T, f>::exists(const T& item) const
{
	size_t pos = ((f(item) % capacity_) + capacity_) % capacity_;
	while (notfree_[pos] && items_[pos] != item)
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	return notfree_[pos];
}
