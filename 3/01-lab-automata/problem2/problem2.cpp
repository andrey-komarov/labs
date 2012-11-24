#include <fstream>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    ifstream in("problem2.in");
    ofstream out("problem2.out");
    string s;
    in >> s;
    size_t n, m, k;
    in >> n >> m >> k;
    vector<bool> terminal(n);
    for (size_t i = 0; i < k; i++)
    {
        size_t t;
        in >> t;
        terminal[t - 1] = true;
    }
    vector<vector<vector<size_t> > > go(n, vector<vector<size_t> >(26, 
                vector<size_t>()));
    for (size_t i = 0; i < m; i++)
    {
        size_t from, to;
        char ch;
        in >> from >> to >> ch;
        go[from - 1][ch - 'a'].push_back(to - 1);
    }
    vector<bool> canBeHere(n);
    canBeHere[0] = true;
    for (size_t i = 0; i < s.size(); i++)
    {
        char ch = s[i] - 'a';
        vector<bool> newCanBeHere(n);
        for (size_t from = 0; from < n; from++)
        {
            if (!canBeHere[from])
                continue;
            for (size_t j = 0; j < go[from][ch].size(); j++)
            {
                size_t to = go[from][ch][j];
                newCanBeHere[to] = true;
            }
        } 
        canBeHere = newCanBeHere;
    }
    for (size_t i = 0; i < n; i++)
    {
        if (canBeHere[i] && terminal[i])
        {
            out << "Accepts" << endl;
            return 0;
        }
    }
    out << "Rejects" << endl;
}
