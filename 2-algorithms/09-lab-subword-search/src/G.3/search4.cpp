#include <fstream>
#include <string>
#include <queue>
#include <cstdio>

using namespace std;

struct node
{
	static const size_t LETTERS = 26;
	typedef node* pnode;
	bool terminal;
	pnode to[LETTERS];
	pnode pi, parent, superterminal;
	int used;
	bool reached;

	node()
	{
		for (size_t i = 0; i < LETTERS; i++)
			to[i] = NULL; // :(
		pi = parent = superterminal = NULL;
		used = 0;
		reached = false;
	}
};

typedef node::pnode pnode;

pnode insert(pnode& t, const string& s, int id, size_t pos = 0, const pnode& parent = NULL)
{
	if (t == NULL)
		t = pnode(new node());
	t->parent = parent;
	if (pos == s.size())
	{
		t->terminal = true;
		return t;
	}
	return insert(t->to[s[pos] - 'a'], s, id, pos + 1, t);
}

void make_p(pnode& t)
{
	pnode hell(new node());
	for (size_t i = 0; i < node::LETTERS; i++)
		hell->to[i] = t;
	hell->parent = hell;
	hell->pi = hell;
	hell->superterminal = hell;
	t->superterminal = hell;
	t->pi = hell;
	queue<pnode> q;
	q.push(t);
	while (!q.empty())
	{
		pnode now = q.front();
		q.pop();
		for (size_t i = 0; i < node::LETTERS; i++)
			if (now->to[i] != NULL)
			{
				now->to[i]->pi = now->pi->to[i];
				if (now->to[i]->pi->terminal)
					now->to[i]->superterminal = now->to[i]->pi;
				else
					now->to[i]->superterminal = now->to[i]->pi->superterminal;
				q.push(now->to[i]);
			}
			else
				now->to[i] = now->pi->to[i];
	}
}

void proceed(pnode& t, const string& s)
{
	pnode now = t;
	for (size_t i = 0; i <= s.size(); i++)
	{
		pnode tmp = now;
		while (tmp != NULL)
		{
			tmp->reached = true;
			pnode tmp2 = tmp->superterminal;
			tmp->superterminal = NULL;
			tmp = tmp2;
		}
		if (i != s.size())
			now = now->to[s[i] - 'a'];
	}
}

int main()
{
	ifstream in("search4.in");
	ofstream out("search4.out");
	int n;
	in >> n;
	pnode t = NULL;
	vector<pnode> terminals(n);
	for (size_t i = 0; i < n; i++)
	{
		string s;
		in >> s;
		terminals[i] = insert(t, s, i + 1);
	}
	make_p(t);
	string s;
	in >> s;
	proceed(t, s);
	for (size_t i = 0; i < n; i++)
		out << (terminals[i]->reached ? "YES" : "NO") << "\n";
}
