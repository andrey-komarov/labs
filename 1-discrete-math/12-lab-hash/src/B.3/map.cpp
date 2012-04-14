#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

size_t hash(const string& a)
{
	size_t hash = 0;
	for (size_t i = 0; i < a.size(); i++)
		hash = hash * 23917 + (a[i] * 71);
	return hash;
}

template<typename K, typename V, size_t (*f)(const K&) = hash>
struct map
{
	map(size_t capacity);
	~map();
	void put(const K&, const V&);
	void remove(const K&);
	bool exists(const K&) const;
	V get(const K&) const;
private:
	K *keys_;
	V *values_;
	size_t *hashes_;
	bool *notfree_;
	size_t capacity_, size_;
};

int main()
{
	freopen("map.in", "r", stdin);
	freopen("map.out", "w", stdout);
	map<string, string> a(2000000);
	char type[6];
	string key;
	string value;
	while (scanf("%s", type) != EOF)
	{
		cin >> key;
		if (type[0] == 'p') 
		{
			cin >> value;
			a.put(key, value);
		}
		else if (type[0] == 'd')
			a.remove(key);
		else if (type[0] == 'g') 
		{
			if (!a.exists(key))
				printf("none\n");
			else 
				cout << a.get(key) << "\n";
		}
	}
}

template<typename K, typename V, size_t (*f)(const K&)>
map<K, V, f>::map(size_t capacity): capacity_(capacity), size_(0)
{
	keys_ = new K[capacity_];
	values_ = new V[capacity_];
	notfree_ = new bool[capacity_];
	hashes_ = new size_t[capacity_];
}

template<typename K, typename V, size_t (*f)(const K&)>
map<K, V, f>::~map()
{
	delete[] keys_;
	delete[] values_;
	delete[] hashes_;
	delete[] notfree_;
}

template<typename K, typename V, size_t (*f)(const K&)>
void map<K, V, f>::put(const K& key, const V& value)
{
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (notfree_[pos] && (hashes_[pos] != hash || keys_[pos] != key))
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	size_ += notfree_[pos] ? 0 : 1;
	notfree_[pos] = true;
	values_[pos] = value;
	keys_[pos] = key;
	hashes_[pos] = hash;
}

template<typename K, typename V, size_t (*f)(const K&)>
void map<K, V, f>::remove(const K& key)
{
	if (!exists(key))
		return;
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (hashes_[pos] != hash || keys_[pos] != key)
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	vector<K> keystoadd;
	vector<V> valuestoadd;
	notfree_[pos] = false;
	pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	size_--;
	while (notfree_[pos])
	{
		keystoadd.push_back(keys_[pos]);
		valuestoadd.push_back(values_[pos]);
		notfree_[pos] = false;
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
		size_--;
	}
	for (size_t i = 0; i < keystoadd.size(); i++)
		put(keystoadd[i], valuestoadd[i]);
}

template<typename K, typename V, size_t (*f)(const K&)>
bool map<K, V, f>::exists(const K& key) const
{
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (notfree_[pos] && hashes_[pos] != hash && keys_[pos] != key)
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	return notfree_[pos];
}

template<typename K, typename V, size_t (*f)(const K&)>
V map<K, V, f>::get(const K& key) const
{
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (notfree_[pos] && hashes_[pos] != hash && keys_[pos] != key)
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	return values_[pos];
}

