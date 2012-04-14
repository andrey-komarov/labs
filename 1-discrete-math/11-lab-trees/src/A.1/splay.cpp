#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

template<typename T>
struct tree {
	virtual void insert(T) = 0;
	virtual bool find(T) = 0;
	virtual void remove(T) = 0;
	virtual T next(T) = 0;
	virtual T prev(T) = 0;
	virtual bool isempty() = 0;
	virtual bool hasnext(T) = 0;
	virtual bool hasprev(T) = 0;
};

template<typename T>
struct splaytree: tree<T> {
	splaytree();
	void insert(T);
	void remove(T);
	bool find(T);
	T next(T);
	T prev(T);
	T max();
	T min();
	bool isempty();
	bool hasnext(T);
	bool hasprev(T);
	class treeisempty {};
	void print(ostream&) const;
	struct node;
	typedef node* pnode;
	pnode root_;
	void print(ostream&, const pnode&) const;

private:
	enum turn {
		RIGHT, LEFT
	};

	void splay(T);
	T min(pnode&);
	T max(pnode&);
	void insert(pnode&, T);
	void merge(pnode&, pnode&, pnode&);
	void swap(T&, T&);
	void zigleft(pnode&);
	void zigright(pnode&);
	void zigzigleft(pnode&);
	void zigzigright(pnode&);
	void zigzagleft(pnode&);
	void zigzagright(pnode&);
};

template<typename T>
ostream& operator<<(ostream&, const splaytree<T>&);

int main() {
	splaytree<int> t;
	ifstream in("bstsimple.in");
	ofstream out("bstsimple.out");
	string type;
	int n;
	while (in >> type >> n) {
		if (type == "insert")
			t.insert(n);
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

template<typename T>
splaytree<T>::splaytree(): root_(NULL) {}

template<typename T>
struct splaytree<T>::node {
	node(T);
	T value();
	T value_;
	pnode left_, right_;
};

template<typename T>
splaytree<T>::node::node(T value): value_(value), left_(NULL), right_(NULL) { }

template<typename T>
T splaytree<T>::node::value() {
	return value_;
}

// ===============================================================

template<typename T>
void splaytree<T>::swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
}

template<typename T>
void splaytree<T>::zigleft(pnode& p) {
	swap(p->value_, p->left_->value_);
	pnode alpha = p->right_;
	p->right_= p->left_;
	p->left_= p->right_->left_;
	p->right_->left_ = p->right_->right_;
	p->right_->right_= alpha;
}

template<typename T>
void splaytree<T>::zigright(pnode& p) {
	swap(p->value_, p->right_->value_);
	pnode alpha = p->left_;
	p->left_ = p->right_;
	p->right_ = p->left_->right_;
	p->left_->right_ = p->left_->left_;
	p->left_->left_ = alpha;
}

template<typename T>
void splaytree<T>::zigzigleft(pnode& p) {
	zigleft(p);
	zigleft(p);
}

template<typename T>
void splaytree<T>::zigzigright(pnode& p) {
	zigright(p);
	zigright(p);
}

template<typename T>
void splaytree<T>::zigzagleft(pnode& p) {
	zigright(p->left_);
	zigleft(p);
}

template<typename T>
void splaytree<T>::zigzagright(pnode& p) {
	zigleft(p->right_);
	zigright(p);
}

template<typename T>
void splaytree<T>::insert(T a) {
	if (find(a)) {
		splay(a);
		return;
	}
	insert(root_, a);
	splay(a);
}

template<typename T>
T splaytree<T>::min() {
	return min(root_);
}

template<typename T>
T splaytree<T>::max() {
	return max(root_);
}

template<typename T>
T splaytree<T>::min(pnode& root) {
	if (root == NULL)
		throw treeisempty();
	pnode now = root;
	while (now->left_ != NULL) {
		now = now->left_;
	}
	T ans = now->value();
	splay(ans);
	return ans;
}

template<typename T>
T splaytree<T>::max(pnode& root) {
	if (root == NULL)
		throw treeisempty();
	pnode now = root;
	while (now->right_ != NULL) {
		now = now->right_;
	}
	T ans = now->value();
	splay(ans);
	return ans;
}

template<typename T>
void splaytree<T>::merge(pnode& left, pnode& right, pnode& root) {
	if (left == NULL) {
		root = right;
		return;
	}
	root = left;
	max(root);
	root->right_ = right;

}

template<typename T>
void splaytree<T>::insert(pnode& to, T what) {
	if (to == NULL) {
		to = new node(what);
	} else
	if (to->value() < what)
		insert(to->right_, what);
	else
		insert(to->left_, what);
}

template<typename T>
bool splaytree<T>::find(const T item) {
	splay(item);
	if (root_ != NULL)
		return root_->value() == item;
	return false;
}

template<typename T>
void splaytree<T>::remove(T a) {
	if (!find(a))
		return;
	splay(a);
	if (root_ != NULL && root_->value() == a) {
		pnode tmp = root_;
		merge(root_->left_, root_->right_, root_);
		delete tmp;
	}
}

template<typename T>
void splaytree<T>::splay(T item) {
	std::vector<pnode> path;
	std::vector<turn> turns;
	pnode now = root_;
	while (now != NULL && now->value() != item) {
		path.push_back(now);
		if (item > now->value()) {
			turns.push_back(RIGHT);
			now = now->right_;
		} else {
			turns.push_back(LEFT);
			now = now->left_;
		}
	}
	if (now == NULL)
		return;
	for (size_t n = path.size(); n > 1; n -= 2) {
		if (turns[n - 2] == LEFT) {
			if (turns[n - 1] == LEFT)
				zigzigleft(path[n - 2]);
			else
				zigzagleft(path[n - 2]);
		} else {
			if (turns[n - 1] == RIGHT)
				zigzigright(path[n - 2]);
			else
				zigzagright(path[n - 2]);
		}
	}
	if (path.size() % 2 == 1) {
		if (turns[0] == LEFT)
			zigleft(path[0]);
		else
			zigright(path[0]);
	}
}


template<typename T>
void splaytree<T>::print(ostream& out) const {
	out << root_;
}

template<typename T>
void splaytree<T>::print(ostream& out, const splaytree<T>::pnode& a) const {
	if (a == NULL) {
		return;
	}
	out << "[";
	print(out, a->left_);
	out << a->value() << " ";
	print(out, a->right_);
	out << "]";
}

template<typename T>
ostream& operator<<(ostream& out, const splaytree<T>& t) {
	t.print(out, t.root_);
	return out;
}

template<typename T>
T splaytree<T>::next(T item) {
	bool inserted = false;
	if (!find(item)) {
		insert(item);
		inserted = true;
	}
	splay(item);
	pnode now = root_->right_;
	while (now->left_ != NULL)
		now = now->left_;
	T ans = now->value();
	splay(ans);
	if (inserted)
		remove(item);
	return ans;
}

template<typename T>
T splaytree<T>::prev(T item) {
	bool inserted = false;
	if (!find(item)) {
		insert(item);
		inserted = true;
	}
	splay(item);
	pnode now = root_->left_;
	while (now->right_ != NULL)
		now = now->right_;
	T ans = now->value();
	splay(ans);
	if (inserted)
		remove(item);
	return ans;
}

template<typename T>
bool splaytree<T>::isempty() {
	return root_ == NULL;
}

template<typename T>
bool splaytree<T>::hasnext(T item) {
	if (isempty())
		return false;
	pnode now = root_;
	while (now->right_ != NULL)
		now = now->right_;
	splay(now->value());
	return item < root_->value();
}

template<typename T>
bool splaytree<T>::hasprev(T item) {
	if (isempty())
		return false;
	pnode now = root_;
	while (now->left_ != NULL)
		now = now->left_;
	splay(now->value());
	return item > root_->value();
}
