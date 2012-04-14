#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

struct tree {
	tree();
	void movetofront(int l, int r);
	void append(int x);	
private:
	struct node;
	typedef node* pnode;
	pnode t_;
	void split(pnode t, pnode& t1, pnode& t2, int x);
	void merge(pnode& t1, pnode& t2, pnode& t);	
	void update(pnode& t);
	friend ostream& operator<<(ostream&, const tree&);
	friend ostream& operator<<(ostream&, const tree::pnode&);
};

int main() {
	ifstream in("movetofront.in");
	ofstream out("movetofront.out");
	int n, m;
	in >> n >> m;
	srand(123);
	tree t;
	for (int i = 0; i < n; i++)
		t.append(i + 1);
	for (int i = 0; i < m; i++) {
		int l, r;
		in >> l >> r;
		t.movetofront(l - 1, r);
	}
	out << t;
}


struct tree::node {
	int x_, y_, cnt_;
	tree::pnode l_, r_;
	node();
	node(int);
	void update();
	friend ostream& operator<<(ostream&, const tree::node&);
};

tree::node::node() {}

tree::node::node(int x): x_(x), y_(rand()), cnt_(1), l_(NULL), r_(NULL) { }

ostream& operator<<(ostream& out, const tree::pnode& t) {
	if (t->l_ != NULL)
		out << t->l_;
	out << t->x_ << " ";
	if (t->r_ != NULL)
		out << t->r_;
	return out;
}

void tree::node::update() {
	int cnt = 0;
	if (l_ != NULL)
		cnt += l_->cnt_;
	if (r_ != NULL)
		cnt += r_->cnt_;
	cnt_ = cnt + 1;
}

tree::tree(): t_(NULL) {}

ostream& operator<<(ostream& out, const tree& t) {
	out << t.t_;
	return out;
}

void tree::update(pnode& t) {
	if (t != NULL)
		t->update();
}

void tree::split(pnode t, pnode& t1, pnode& t2, int x) {
	if (t == NULL) {
		t1 = t2 = NULL;
		return;
	}
	int l = t->l_ == NULL ? 0 : t->l_->cnt_;
	if (x <= l) {
		split(t->l_, t1, t->l_, x);	
		t2 = t;
	} else {
		split(t->r_, t->r_, t2, x - l - 1);
		t1 = t;
	}
	update(t1);
	update(t2);
}

void tree::merge(pnode& t1, pnode& t2, pnode& t) {
	if (t1 == NULL || t2 == NULL) {
		t = t1 == NULL ? t2 : t1;
		return;
	}
	if (t1->y_ > t2->y_) {
		merge(t1->r_, t2, t1->r_);
		t = t1;
	} else {
		merge(t1, t2->l_, t2->l_);
		t = t2;
	}
	update(t);
}

void tree::append(int x) {
	pnode a = new node(x);
	merge(t_, a, t_);
}

void tree::movetofront(int l, int r) {
	pnode t1, t2, t3;
	split(t_, t2, t3, r);
	split(t2, t1, t2, l);
	merge(t2, t1, t2);
	merge(t2, t3, t_);
}
