#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

static const size_t ALPHA = 26;
#define nullptr NULL

struct trie
{
	typedef trie* ptr;
	ptr edges[ALPHA];
	size_t id;
	static size_t ID;
	trie() : id(ID++)
	{
		fill(edges, edges + ALPHA, (ptr)nullptr);
	}
};

void insert(trie::ptr& t, string s)
{
	size_t n = s.size();
	trie::ptr now = t;
	for (size_t i = 0; i < n; i++)
	{
		if (now->edges[s[i] - 'a'] == nullptr)
			now->edges[s[i] - 'a'] = new trie();
		now = now->edges[s[i] - 'a'];
	}
}

size_t trie::ID = 1;

void traverse(const trie::ptr& t, ostream& out)
{
	for (size_t i = 0; i < ALPHA; i++)
		if (t->edges[i] != nullptr)
		{
			out << t->id << " " << t->edges[i]->id << " " << char(i + 'a') << "\n";
			traverse(t->edges[i], out);
		}
}

int main()
{
	ifstream in("trie.in");
	ofstream out("trie.out");
	string s;
	in >> s;
	size_t n = s.size();
	trie::ptr t = new trie();
	for (size_t i = 0; i < n; i++)
		insert(t, s.substr(i, n - i));
	out << (trie::ID - 1) << " " << (trie::ID - 2) << "\n";
	traverse(t, out);
}
