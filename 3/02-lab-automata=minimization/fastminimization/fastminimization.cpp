#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <tr1/unordered_set>
#include <tr1/unordered_map>
#include <queue>

using namespace std;
using namespace std::tr1;

struct automata
{
    static const char ALPHABET = 26;
    size_t n, start, t;
    vector<bool> terminal;
    vector<unordered_map<char, size_t> > g;

    automata() {}

    automata(size_t start, const vector<bool>& terminal, 
             const vector<unordered_map<char, size_t> >& g):
        start(start),
        t(0),
        terminal(terminal),
        g(g)
    {
        n = terminal.size();
        for (size_t i = 0; i < n; i++)
            t += terminal[i];
    }

    void addHell();
    void removeHell();
    void minimize();
    void swap(automata&);
    void swapIDs(size_t, size_t);
    void setStartZerothVertex();
    void killUnreachable();
};

istream& operator>>(istream& in, automata& a)
{
    size_t n, m, k;
    in >> n >> m >> k;
    vector<bool> terminal(n, false);
    for (size_t i = 0; i < k; i++)
    {
        size_t a;
        in >> a;
        terminal[a - 1] = true;
    }
    vector<unordered_map<char, size_t> > g(n);
    for (size_t i = 0; i < m; i++)
    {
        size_t a, b;
        char ch;
        in >> a >> b >> ch;
        g[a - 1][ch] = b - 1;
    }
    a = automata(0, terminal, g);
    return in;
}

ostream& operator<<(ostream& out, const automata& a)
{
    size_t m = 0;
    for (vector<unordered_map<char, size_t> >::const_iterator it = a.g.begin(); it != a.g.end(); it++)
        m += it->size();
    size_t k = 0;
    for (size_t i = 0; i < a.terminal.size(); i++)
        k += a.terminal[i];
    size_t n = a.terminal.size();
    out << n << " " << m << " " << k << "\n";
    for (size_t i = 0; i < n; i++)
        if (a.terminal[i])
            out << (i+1) << " ";
    out << "\n";
    for (size_t i = 0; i < n; i++)
        for (unordered_map<char, size_t>::const_iterator it = a.g[i].begin(); it != a.g[i].end(); it++)
            out << (i+1) << " " << (it->second + 1) << " " << it->first << "\n";    
    return out;
}

int main()
{
    ifstream in("fastminimization.in");
    ofstream out("fastminimization.out");
    automata a;
    in >> a;
    a.minimize();
    out << a;
}

void automata::minimize() 
{
    killUnreachable();
    addHell();
    vector<size_t> eqlass(n);
    vector<unordered_set<size_t> > classes(2);
    vector<bool> inQueue(2, true);
    vector<bool> alive(2, true);
    for (size_t i = 0; i < n; i++)
    {
        eqlass[i] = terminal[i];
        classes[eqlass[i]].insert(i);
    }
    vector<unordered_map<char, vector<size_t> > > gInv(n);
    for (size_t i = 0; i < n; i++)
        for (unordered_map<char, size_t>::const_iterator it = g[i].begin(); it != g[i].end(); it++)
            gInv[it->second][it->first].push_back(i);
    vector<size_t> q;
//    q.push_back(0);
//    q.push_back(1);
    if (classes[0].size() < classes[1].size())
        q.push_back(0);
    else
        q.push_back(1);
    int p1 = 0, p2 = 1;
    while (p1 <= p2)
    {
        size_t sID = q[p1++];
        if (!inQueue[sID])
            continue;
        inQueue[sID] = false;
        unordered_set<size_t> codomain = classes[sID];
        unordered_map<size_t, size_t> timesUsed;
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        {
            unordered_set<size_t> domain;
            for (unordered_set<size_t>::const_iterator it = codomain.begin(); it != codomain.end(); it++)
            {
                size_t v = *it;
                for (vector<size_t>::const_iterator it2 = gInv[v][ch].begin(); it2 != gInv[v][ch].end(); it2++)
                {
                    size_t p = *it2;
                    domain.insert(eqlass[p]);
                    timesUsed[eqlass[p]]++;
                }
            }
            for (unordered_set<size_t>::const_iterator it2 = domain.begin(); it2 != domain.end(); it2++)
            {
                size_t splitMe = *it2;
                if (classes[splitMe].size() == timesUsed[splitMe])
                    continue;
                unordered_set<size_t> r1, r2;
                for (unordered_set<size_t>::const_iterator it3 = classes[splitMe].begin(); it3 != classes[splitMe].end(); it3++)
                {
                    size_t v = *it3;
                    if (g[v].count(ch) && codomain.count(g[v][ch]))
                       r1.insert(v);
                    else
                       r2.insert(v);
                }
                if (r1.size() != 0 && r2.size() != 0)
                {
                    inQueue.push_back(true);
                    inQueue.push_back(true);
                    alive.push_back(true);
                    alive.push_back(true);
                    size_t r1ID = classes.size();
                    classes.push_back(r1);
                    size_t r2ID = classes.size();
                    classes.push_back(r2);
                    classes[splitMe].clear();
                    alive[splitMe] = false;
                    for (unordered_set<size_t>::const_iterator it = r1.begin(); it != r1.end(); it++)
                        eqlass[*it] = r1ID;
                    for (unordered_set<size_t>::const_iterator it = r2.begin(); it != r2.end(); it++)
                        eqlass[*it] = r2ID;
                    if (inQueue[splitMe])
                    {
                        inQueue[splitMe] = false;
                        q.push_back(r1ID);
                        q.push_back(r2ID);
                        p2 += 2;
                    } else {
                        if (r1.size() < r2.size())
                        {
                            q.push_back(r1ID);
                            p2++;
                        } else {
                            q.push_back(r2ID);
                            p2++;
                        }
                    }
                }
            }
        }
}
    vector<size_t> newID(alive.size(), -1);
    size_t vcnt = 0;
    for (size_t i = 0; i < alive.size(); i++)
        if (alive[i])
            newID[i] = vcnt++;
    vector<bool> newTerminal(vcnt, false);
    for (size_t i = 0; i < n; i++)
        if (terminal[i])
            newTerminal[newID[eqlass[i]]] = true;
    vector<unordered_map<char, size_t> > newG(vcnt);
    for (size_t i = 0; i < n; i++)
        for (unordered_map<char, size_t>::const_iterator it = g[i].begin(); it != g[i].end(); it++)
            newG[newID[eqlass[i]]][it->first] = newID[eqlass[it->second]];
    automata newAutomata(newID[eqlass[start]], newTerminal, newG);
    swap(newAutomata);
    removeHell();
    setStartZerothVertex();
}

void automata::swap(automata& a)
{
    std::swap(start, a.start);
    std::swap(g, a.g);
    std::swap(terminal, a.terminal);
    std::swap(n, a.n);
    std::swap(t, a.t);
}

void automata::addHell()
{
    g.push_back(unordered_map<char, size_t>());
    terminal.push_back(false);
    size_t hell = n;
    n++;
    for (size_t i = 0; i < n; i++)
    {
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
            if (!g[i].count(ch))
                g[i][ch] = hell;
    }
    for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        g[hell][ch] = hell;
}

void automata::removeHell()
{
    size_t hell = n + 1;
    for (size_t i = 0; i < n; i++)
    {
        bool good = false;
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        {
            if (g[i][ch] != i)
            {
                good = true;
                break;
            }
        }
        if (!good)
        {
            hell = i;
            break;
        }
    }
    swapIDs(hell, n - 1);
    g.pop_back();
    terminal.pop_back();
    n--;
    for (size_t i = 0; i < n; i++)
    {
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        {
            if (g[i][ch] == n)
                g[i].erase(ch);
        }
    }
}

void automata::setStartZerothVertex()
{
    swapIDs(start, 0);
    start = 0;
}

void automata::swapIDs(size_t v1, size_t v2)
{
    for (size_t i = 0; i < n; i++)
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        {
            if (!g[i].count(ch))
                continue;
            if (g[i][ch] == v1)
                g[i][ch] = v2;
            else if (g[i][ch] == v2)
                g[i][ch] = v1;
        }
    std::swap(g[v1], g[v2]);
    bool tmp = terminal[v1];
    terminal[v1] = terminal[v2];
    terminal[v2] = tmp;
    if (start == v1)
        start = v2;
    else if (start == v2)
        start = v1;
}

void automata::killUnreachable()
{
    queue<size_t> q;
    vector<bool> used(n);
    used[start] = true;
    q.push(start);
    while (!q.empty())
    {
        size_t now = q.front();
        q.pop();
        for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
        {
            if (!g[now].count(ch))
                continue;
            size_t to = g[now][ch];
            if (!used[to])
            {
                used[to] = true;
                q.push(to);
            }
        }
    }
    for (size_t i = 0; i < n; i++)
        if (!used[i])
        {
            for (char ch = 'a'; ch < 'a' + ALPHABET; ch++)
                g[i][ch] = i;
            terminal[i] = false;
        }        
}
