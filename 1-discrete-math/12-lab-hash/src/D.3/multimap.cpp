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
struct multimap
{
	multimap(size_t capacity);
	~multimap();
	void put(const K&, const V&);
	void remove(const K&, const V&);
	bool exists(const K&, const V&) const;
	void deleteall(const K&);
	vector<V> get(const K&) const;
private:
	K *keys_;
	V *values_;
	size_t *hashes_;
	bool *notfree_;
	size_t capacity_, size_;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>&);

int main()
{
	freopen("multimap.in", "r", stdin);
	freopen("multimap.out", "w", stdout);
	multimap<string, string> a(2000000);
	string type;
	string key;
	string value;
	while (cin >> type)
	{
		cin >> key;
		switch (type[0])
		{
			case 'p':
				cin >> value;
				a.put(key, value);
				break;
			case 'g':
				cout << a.get(key) << "\n"; 
				break;
			case 'd':
				if (type.size() == 6)
				{
					cin >> value;
					a.remove(key, value);
				}
				else 
				{
					a.deleteall(key);
				}
				break;
		}
	}
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a)
{
	out << a.size() << " ";
	for (size_t i = 0; i < a.size(); i++)
		out << a[i] << " ";
	return out;
}

template<typename K, typename V, size_t (*f)(const K&)>
multimap<K, V, f>::multimap(size_t capacity): capacity_(capacity), size_(0)
{
	keys_ = new K[capacity_];
	values_ = new V[capacity_];
	notfree_ = new bool[capacity_];
	hashes_ = new size_t[capacity_];
}

template<typename K, typename V, size_t (*f)(const K&)>
multimap<K, V, f>::~multimap()
{
	delete[] keys_;
	delete[] values_;
	delete[] hashes_;
	delete[] notfree_;
}

template<typename K, typename V, size_t (*f)(const K&)>
bool multimap<K, V, f>::exists(const K& key, const V& value) const
{
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (notfree_[pos] && (hashes_[pos] != hash || keys_[pos] != key || values_[pos] != value))
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	return notfree_[pos];
}

template<typename K, typename V, size_t (*f)(const K&)>
void multimap<K, V, f>::put(const K& key, const V& value)
{
	if (exists(key, value)) {
		return;
	}
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (notfree_[pos])
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	size_++;
	notfree_[pos] = true;
	values_[pos] = value;
	keys_[pos] = key;
	hashes_[pos] = hash;
}

template<typename K, typename V, size_t (*f)(const K&)>
void multimap<K, V, f>::remove(const K& key, const V& value)
{
	if (!exists(key, value))
		return;
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	while (hashes_[pos] != hash || keys_[pos] != key || values_[pos] != value) {
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	}
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
vector<V> multimap<K, V, f>::get(const K& key) const
{
	size_t hash = f(key);
	size_t pos = ((hash % capacity_) + capacity_) % capacity_;
	vector<V> ans;
	while (notfree_[pos]) {
		if (hashes_[pos] == hash && keys_[pos] == key)
			ans.push_back(values_[pos]);
		pos = (pos == capacity_ - 1) ? 0 : (pos + 1);
	}
	return ans;
}

template<typename K, typename V, size_t (*f)(const K&)>
void multimap<K, V, f>::deleteall(const K& key)
{
	vector<V> values = get(key);
	for (size_t i = 0; i < values.size(); i++)
		remove(key, values[i]);
}
