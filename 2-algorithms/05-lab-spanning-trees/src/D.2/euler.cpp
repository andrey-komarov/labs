#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct edge
{
    int from, to;
    edge* rev;
    bool enabled;

    edge(int from, int to) : from(from), to(to), enabled(true) {}

    void disable()
    {
        enabled = rev->enabled = false;
    }
};

vector<size_t> start;
vector<int> ans;
vector<vector<edge*> > g;

void euler(int v)
{
    for (; start[v] < g[v].size(); start[v]++)
    {
        edge* e = g[v][start[v]];
        if (e->enabled)
        {
            e->disable();
            euler(e->to);
        }
    }
    ans.push_back(v);
}

int main()
{
    ifstream in("euler.in");
    ofstream out("euler.out");
    size_t n, m = 0, odd = 0;
    in >> n;
    g = vector<vector<edge*> >(n, vector<edge*>());
    start = vector<size_t>(n, 0);
    for (size_t i = 0; i < n; i++)
    {
        size_t size;
        in >> size;
        m += size;
        odd += size % 2;
        for (size_t j = 0; j < size; j++)
        {
            size_t a;
            in >> a;
            a--;
            if (a > i)
            {
                edge* e1 = new edge(a, i);
                edge* e2 = new edge(i, a);
                e1->rev = e2;
                e2->rev = e1;
                g[i].push_back(e2);
                g[a].push_back(e1);
            }
        }
    }

    size_t start = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (g[i].size() % 2 == 1)
            start = i;
    }

    euler(start);
    if (ans.size() == (m / 2) + 1 && odd <= 2)
    {
        out << (ans.size() - 1) << "\n";
        for (size_t i = 0; i < ans.size(); i++)
            out << (ans[i] + 1) << " ";
    }
    else
        out << "-1\n";
}

