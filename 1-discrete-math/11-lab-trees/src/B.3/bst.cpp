#include <fstream>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <string>

using namespace std;

class assert{};

struct tree {
	tree();
	tree& add(int);
	bool find(int);
	tree& remove(int);
	int min();
	int max();
	int next(int);
	int prev(int);
	bool hasnext(int);
	bool hasprev(int);
	bool empty();
	~tree();
private:
	struct node_interface;
	typedef node_interface* pnode;
	struct node;
	struct leaf;
	node* root_;
	void fixroot();
	friend bool operator<(const node_interface&, const node_interface&);
	friend ostream& operator<<(ostream&, const tree&);
	friend ostream& operator<<(ostream&, const node&);
	friend ostream& operator<<(ostream&, const leaf&);
};

int main() {
	ifstream in("bst.in");
	ofstream out("bst.out");
	string type;
	int n;
	tree t;
	while (in >> type >> n) {
		if (type == "insert")
			t.add(n);
		else if (type == "exists")
			out << (t.find(n) ? "true" : "false") << "\n";
		else if (type == "delete")
			t.remove(n);
		else if (type == "next") {
			if (t.hasnext(n))
				out << t.next(n) << "\n";
			else
				out << "none\n";
		} else if (type == "prev") {
			if (t.hasprev(n))
				out << t.prev(n) << "\n";
			else
				out << "none\n";
		}
	}
}

struct tree::node_interface {
	virtual bool isleaf() const = 0;
	virtual int value() const = 0;
	virtual int count() const = 0;
	virtual void update() = 0;
	virtual void print(ostream&) const = 0;
	virtual int min() = 0;
	virtual int max() = 0;
};

bool operator<(const tree::node_interface& first, const tree::node_interface& second) {
	return first.value() < second.value();
}

// ============================================

struct tree::leaf: tree::node_interface {
	leaf();
	leaf(int);
	int value() const;
	bool isleaf() const;
	int count() const;
	int min();
	int max();
	friend ostream& operator<<(ostream&, const leaf&);
private:
	int value_;
	void print(ostream&) const;
	void update();
};

tree::leaf::leaf() {}

tree::leaf::leaf(int value): value_(value) {}

int tree::leaf::value() const {
	return value_;
}

// ааа, капитан очевидность в моей программе
bool tree::leaf::isleaf() const {
	return true;
}

int tree::leaf::count() const {
	return 0;
}

void tree::leaf::update() {}

int tree::leaf::min() {
	return value();
}

int tree::leaf::max() {
	return value();
}

void tree::leaf::print(ostream& out) const {
	out << value();
}

ostream& operator<<(ostream& out, const tree::leaf& item) {
	item.print(out);
	return out;
}

// ===========================================

struct tree::node: tree::node_interface {
	node();
	node(pnode&, pnode&);
	node(node*&, node*&);
	bool find(const pnode&);
	void insert(pnode&);
	void remove(pnode&);
	bool isleaf() const;
	int value() const;
	int count() const;
	int min();
	int max();
	bool empty();
	int next(pnode&);
	int prev(pnode&);
	void clear();
	void split(pnode&, pnode&);
private:
	int cnt_;
	int max_[4];
	pnode nodes_[4];

	void fix();
	void fixsplit(int);
	void fixmerge(int);
	void update();
	void inserthere(pnode&);
	void removefromhere(pnode&);
	pnode& findwhere(const pnode& item);

	void print(ostream&) const;
	friend ostream& operator<<(ostream&, const node&);
	friend void tree::fixroot();
};

bool tree::node::isleaf() const {
	return false;
}

int tree::node::value() const {
	return max_[cnt_ - 1];
}

int tree::node::count() const {
	return cnt_;
}

tree::node::node(): cnt_(0) {}

tree::node::node(pnode& a, pnode& b): cnt_(2) {
	nodes_[0] = a;
	nodes_[1] = b;
	nodes_[2] = NULL;
	nodes_[3] = NULL;
	update();
}

tree::node::node(node*& a, node*& b) {
	cnt_ = a->count() + b->count();
	int i;
	for (i = 0; i < a->count(); i++)
		nodes_[i] = a->nodes_[i];
	for (int j = 0; j < b->count(); j++, i++)
		nodes_[i] = b->nodes_[j];
	update();
}

void tree::node::split(pnode& first, pnode& second) {
	first = new node(nodes_[0], nodes_[1]);
	second = new node(nodes_[2], nodes_[3]);
}

void tree::node::inserthere(pnode& item) {
	int i;
	for (i = 0; i < cnt_ && *nodes_[i] < *item; i++);
	for (int j = cnt_ - 1; j >= i; j--)
		nodes_[j + 1] = nodes_[j];
	nodes_[i] = item;
	cnt_++;
}

void tree::node::removefromhere(pnode& item) {
	int i;
	for (i = 0; i < cnt_ && *nodes_[i] < *item; i++);
	delete nodes_[i];
	for (; i < cnt_ - 1; i++)
		nodes_[i] = nodes_[i + 1];
	cnt_--;
}

tree::pnode& tree::node::findwhere(const tree::pnode& item) {
	int i;
	for (i = 0; i < cnt_ - 1 && max_[i] < item->value(); i++);
	return nodes_[i];
}

void tree::node::fix() {
	for (int i = 0; i < cnt_; i++) {
		if (nodes_[i]->count() == 4)
			fixsplit(i);
		if (nodes_[i]->count() == 1 && cnt_ > 1) {
			if (i == 0)
				fixmerge(0);
			else
				fixmerge(i - 1);
		}
	}
}

void tree::node::fixsplit(int pos) {
	node* tmp = (node*)nodes_[pos];
	for (int i = cnt_ - 1; i >= pos + 1; i--)
		nodes_[i + 1] = nodes_[i];
	tmp->split(nodes_[pos], nodes_[pos + 1]);
	delete tmp;
	cnt_++;
	update();
}

void tree::node::fixmerge(int pos) {
	node* left = (node*)nodes_[pos];
	node* right = (node*)nodes_[pos + 1];
	for (int i = pos + 1; i < cnt_ - 1; i++)
		nodes_[i] = nodes_[i + 1];
	nodes_[pos] = new node(left, right);
	delete left;
	delete right;
	cnt_--;
	update();
}

void tree::node::update() {
	for (int i = 0; i < cnt_; ++i) {
		max_[i] = nodes_[i]->value();
	}
	fix();
}

void tree::node::print(ostream& out) const {
	out << "[";
	for (int i = 0; i < cnt_ - 1; i++) {
		nodes_[i]->print(out);
		out << " ";
	}
	if (cnt_ != 0)
		nodes_[cnt_ - 1]->print(out);
	out << "]";
}

ostream& operator<<(ostream& out, const tree::node& item) {
	item.print(out);
	return out;
}

void tree::node::insert(pnode& item) {
	if (count() == 0 || nodes_[0]->isleaf()) {
		inserthere(item);
	} else {
		((node*)findwhere(item))->insert(item);
	}
	update();
	fix();
}

bool tree::node::find(const pnode& item) {
	if (isleaf())
		return item->value() == value();
	else if (count() == 0)
		return false;
	else
		return ((node*)findwhere(item))->find(item);
}

void tree::node::remove(pnode& item) {
	if (count() == 0 || isleaf())
		return;
	if (nodes_[0]->isleaf()) {
		removefromhere(item);
	} else {
		((node*)findwhere(item))->remove(item);
	}
	update();
	fix();
}

bool tree::node::empty() {
	return cnt_ == 0;
}

int tree::node::min() {
	pnode now = this;
	while (!now->isleaf())
		now = ((node*)now)->nodes_[0];
	return now->value();
}

int tree::node::max() {
	pnode now = this;
	while (!now-isleaf())
		now = ((node*)now)->nodes_[now->count()];
	return now->value();
}

int tree::node::next(pnode& item) {
	pnode goodparent = NULL;
	pnode now = this;
	while (!now->isleaf()) {
		int i;
		for (i = 0; *(((node*)now)->nodes_[i]) < *item; i++);
		if (i < now->count() - 1)
			goodparent = ((node*)now)->nodes_[i + 1];
		now = ((node*)now)->nodes_[i];
	}
	return goodparent->min();
}

int tree::node::prev(pnode& item) {
	pnode goodparent = NULL;
	pnode now = this;
	while (!now->isleaf()) {
		int i;
		for (i = 0; *(((node*)now)->nodes_[i]) < *item; i++);
		if (i > 0)
			goodparent = ((node*)now)->nodes_[i - 1];
		now = ((node*)now)->nodes_[i];
	}
	return goodparent->max();
}

// =============================================

tree::tree() {
	root_ = new node();
}

tree::~tree() {
	delete root_;
}

void tree::fixroot() {
	if (root_->count() == 4) {
		pnode first, second;
		root_->split(first, second);
		delete root_;
		root_ = new node(first, second);
	}
	if (root_->count() == 1) {
		pnode tmp = root_;
		if (!root_->nodes_[0]->isleaf()) {
			root_ = (node*)(root_->nodes_[0]);
			delete tmp;
			fixroot();
		}
	}
}

tree& tree::add(int item) {
	pnode newleaf = new leaf(item);
	if (!find(item))
		root_->insert(newleaf);
	fixroot();
	return *this;
}

bool tree::find(int item) {
	pnode nodeitem = new leaf(item);
	bool result = root_->find(nodeitem);
	delete nodeitem;
	return result;
}

tree& tree::remove(int a) {
	pnode item = new leaf(a);
	if (root_->find(item))
		root_->remove(item);
	delete item;
	fixroot();
	return *this;
}

bool tree::empty() {
	return root_->empty();
}

int tree::min() {
	return root_->min();
}

int tree::max() {
	return root_->max();
}

bool tree::hasnext(int item) {
	return !empty() && max() > item;
}

bool tree::hasprev(int item) {
	return !empty() && min() < item;
}

int tree::next(int a) {
	pnode item = new leaf(a);
	bool inserted = false;
	if (!find(a)) {
		add(a);
		inserted = true;
	}
	int result = root_->next(item);
	if (inserted)
		remove(a);
	delete item;
	return result;
}

int tree::prev(int a) {
	pnode item = new leaf(a);
	bool inserted = false;
	if (!find(a)) {
		add(a);
		inserted = true;
	}
	int result = root_->prev(item);
	if (inserted)
		remove(a);
	delete item;
	return result;
}

ostream& operator<<(ostream& out, const tree& t) {
	out << *(t.root_);
	return out;
}
