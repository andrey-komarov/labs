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
	map(size_t);
	void put(K, V);
	void remove(K);
	V get(K) const;
	V prev(K) const;
	V next(K) const;
	bool exists(K) const;
	bool has_prev(K) const;
	bool has_next(K) const;
private:
	size_t get_pos(K) const;
	struct item;
	item **items_;
	item *last_;
	item* find(item*, const K&) const;
	static void insert(item*&, item*&);
	size_t capacity_, size_;
};

int main()
{
	ifstream in("linkedmap.in");
	ofstream out("linkedmap.out");
	map<string, string> a(200000);
	string type, key, value;
	while (in >> type >> key)
	{
		if (type == "put")
		{
			in >> value;
			a.put(key, value);
		}
		if (type == "delete")
			a.remove(key);
		if (type == "get")
		{
			if (a.exists(key))
				out << a.get(key) << "\n";
			else
				out << "none\n";
		}
		if (type == "prev")
		{
			if (a.has_prev(key))
				out << a.prev(key) << "\n";
			else
				out << "none\n"; 
		}
		if (type == "next")
		{
			if (a.has_next(key))
				out << a.next(key) << "\n";
			else
				out << "none\n";
		}
	}
}

template<typename K, typename V, size_t (*f)(const K&)>
map<K, V, f>::map(size_t capacity): capacity_(capacity), size_(0)
{
	items_ = new item*[capacity_];
	for (size_t i = 0; i < capacity_; i++)
		items_[i] = NULL;
	last_ = NULL;
}

template<typename K, typename V, size_t (*f)(const K&)>
struct map<K, V, f>::item
{
	const K key_;
	V value_;
	item *tail_, *prev_, *next_;
	const size_t hash_;
	item() {}
	item(const K& key, const V& value, item*& prev):
		key_(key), value_(value), tail_(NULL), prev_(prev), next_(NULL), hash_(f(key)){}
};

template<typename K, typename V, size_t (*f)(const K&)>
size_t map<K, V, f>::get_pos(K key) const
{
	return ((f(key) % capacity_) + capacity_) % capacity_;
}

template<typename K, typename V, size_t (*f)(const K&)>
typename map<K, V, f>::item* map<K, V, f>::find(item* where, const K& key) const
{
	size_t hash = f(key);
	while (where != NULL && (where->hash_ != hash || where->key_ != key)) {
		where = where->tail_;
	}
	return where;
}

template<typename K, typename V, size_t (*f)(const K&)>
void map<K, V, f>::insert(item*& where, item*& what)
{
	what->tail_ = where;
	std::swap(what, where);
}

template<typename K, typename V, size_t (*f)(const K&)>
void map<K, V, f>::put(K key, V value)
{
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	if (candidate != NULL) 
	{
		candidate->value_ = value;
	} 
	else 
	{
		item *now = new item(key, value, last_);
		if (last_ != NULL)
			last_->next_ = now;
		last_ = now;
		insert(items_[pos], now);
	}
}

template<typename K, typename V, size_t (*f)(const K&)>
V map<K, V, f>::get(K key) const
{
	return find(items_[get_pos(key)], key)->value_;
}

template<typename K, typename V, size_t (*f)(const K&)>
void map<K, V, f>::remove(K key)
{
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	if (candidate == NULL) {
		return;
	}
	if (candidate->prev_ != NULL)
		candidate->prev_->next_ = candidate->next_;
	if (candidate->next_ != NULL)
		candidate->next_->prev_ = candidate->prev_;
	if (candidate == last_) 
	{
		last_ = last_->prev_;
	}
	if (items_[pos] == candidate)
		items_[pos] = items_[pos] ->tail_;
	// memory leak here >_<
	// delete candidate;
}

template<typename K, typename V, size_t (*f)(const K&)>
bool map<K, V, f>::exists(K key) const
{
	size_t pos = get_pos(key);
	return find(items_[pos], key) != NULL;
}

template<typename K, typename V, size_t (*f)(const K&)>
V map<K, V, f>::prev(K key) const 
{
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	return candidate->prev_->value_;
}

template<typename K, typename V, size_t (*f)(const K&)>
bool map<K, V, f>::has_prev(K key) const 
{
	if (!exists(key))
		return false;
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	return candidate->prev_ != NULL;
}

template<typename K, typename V, size_t (*f)(const K&)>
V map<K, V, f>::next(K key) const 
{
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	return candidate->next_->value_;
}

template<typename K, typename V, size_t (*f)(const K&)>
bool map<K, V, f>::has_next(K key) const 
{
	if (!exists(key))
		return false;
	size_t pos = get_pos(key);
	item* candidate = find(items_[pos], key);
	return candidate->next_ != NULL;
}
