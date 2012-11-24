#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ifstream in("start.in");
    ofstream out("start.out");
    size_t n, m;
    in >> m >> n;
    vector<char> let(n);
    vector<vector<size_t> > go(n);
    for (size_t i = 0; i < n; i++)
    {
        size_t to0, to1;
        in >> to0 >> to1 >> let[i];
        go[to0 - 1].push_back(i);
        go[to1 - 1].push_back(i);
    }
    string s;
    in >> s;
    vector<int> ans;
    vector<bool> canBe(n, true);
    for (int i = s.size() - 1; i >= 0; i--)
    {
        char ch = s[i];
        vector<bool> newCanBe(n, false);
        for (size_t vFrom = 0; vFrom < n; vFrom++)
        {
            if (!canBe[vFrom])
                continue;
            for (size_t j = 0; j < go[vFrom].size(); j++)
            {
                int to = go[vFrom][j];
                if (let[vFrom] == ch)
                    newCanBe[to] = true;
            }
        }
        swap(canBe, newCanBe);
    }
    size_t ansCnt = 0;
    for (size_t i = 0; i < n; i++)
        if (canBe[i])
            ansCnt++;
    out << ansCnt << "\n";
    for (size_t i = 0; i < n; i++)
        if (canBe[i])
            out << i+1 << " ";
}
