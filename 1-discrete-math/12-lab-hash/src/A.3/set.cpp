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
	return a;
}

template<typename T, size_t (*f)(const T&) = hash>
struct set
{
	set(size_t capacity = initial_capacity);
	set(const set<T, f>&);
	set(const set<T, f>&, size_t);
	~set();
	set<T, f>& insert(const T&);
	set<T, f>& remove(const T&);
	bool exists(const T&) const;
	void swap(set<T, f>&);
private:
	static const size_t initial_capacity = 8;
	static int find(const vector<T>&, const T&);
	static bool remove(vector<T>&, const T&);
	static bool insert(vector<T>&, const T&);
	void resize(size_t);
	void normup();
	void normdown();
	size_t capacity_, size_;
	vector<T>* items_;
};

size_t MAX = 0;

int main()
{
	freopen("set.in", "r", stdin);
	freopen("set.out", "w", stdout);
	set<int> a;
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
	items_ = new vector<T>[capacity_];
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::~set()
{
	delete[] items_;
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::swap(set<T, f>& other)
{
	std::swap(capacity_, other.capacity_);
	std::swap(size_, other.size_);
	std::swap(items_, other.items_);
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::set(const set<T, f>& other): capacity_(other.capacity_), size_(other.size_)
{
	items_ = new vector<T>[capacity_];
	for (size_t i = 0; i < capacity_; i++)
		items_[i] = other.items_[i];
}

template<typename T, size_t (*f)(const T&)>
set<T, f>::set(const set<T, f>& other, size_t capacity): capacity_(capacity), size_(0)
{
	items_ = new vector<T>[capacity_];
	for (size_t i = 0; i < other.capacity_; i++)
		for (size_t j = 0; j < other.items_[i].size(); j++) {
			insert(other.items_[i][j]);
		}
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::normup()
{
	if (2 * size_ > capacity_)
		resize(capacity_ * 2);
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::normdown()
{
	if (size_ > initial_capacity && 4 * size_ < capacity_)
		resize(capacity_ / 2);
}

template<typename T, size_t (*f)(const T&)>
void set<T, f>::resize(size_t new_size_)
{
	set<T, f> a(*this, new_size_);
	a.swap(*this);
}

template<typename T, size_t (*f)(const T&)>
int set<T, f>::find(const vector<T>& items, const T& item)
{
	for (size_t i = 0; i < items.size(); i++)
		if (items[i] == item)
			return i;
	return -1;
}

template<typename T, size_t (*f)(const T&)>
bool set<T, f>::remove(vector<T>& items, const T& item)
{
	int pos = find(items, item);
	if (pos != -1) {
		items.erase(items.begin() + pos);
		return true;
	}
	return false;
}

template<typename T, size_t (*f)(const T&)>
bool set<T, f>::insert(vector<T>& items, const T& item)
{
	if (find(items, item) == -1)
	{
		MAX = max(MAX, items.size());
		items.push_back(item);
		return true;
	}
	return false;
}

template<typename T, size_t (*f)(const T&)>
bool set<T, f>::exists(const T& item) const
{
	return find(items_[f(item) % capacity_], item) != -1;
}

template<typename T, size_t (*f)(const T&)>
set<T, f>& set<T, f>::insert(const T& item)
{
	size_ += insert(items_[f(item) % capacity_], item) ? 1 : 0;
	normup();
	return *this;
}

template<typename T, size_t (*f)(const T&)>
set<T, f>& set<T, f>::remove(const T& item)
{
	size_ -= remove(items_[f(item) % capacity_], item) ? 1 : 0;
	normdown();
	return *this;
}
