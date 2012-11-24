#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ifstream in("problem1.in");
    ofstream out("problem1.out");
    string s;
    in >> s;
    size_t n, m, k;
    in >> n >> m >> k;
    vector<bool> terminal(n, false);
    for (size_t i = 0; i < k; i++)
    {
        size_t t;
        in >> t;
        terminal[t - 1] = true;
    }
    vector<vector<size_t> > go(n, vector<size_t>(26, -1));
    for (size_t i = 0; i < m; i++) 
    {
        size_t from, to;
        char ch;
        in >> from >> to >> ch;
        go[from - 1][ch - 'a'] = to - 1;
    }
    size_t now = 0;
    for (size_t i = 0; i < s.size(); i++)
    {
        char ch = s[i] - 'a';
        if (go[now][ch] == -1)
        {
            out << "Rejects" << endl;
            return 0;
        }
        now = go[now][ch];
    }
    if (terminal[now])
        out << "Accepts" << endl;
    else
        out << "Rejects" << endl;
}
