#include <iostream>
#include <fstream>
#include <map>
#include <vector>
 
using namespace std;
 
vector<bool> term_1, term_2;
vector<vector<int> > aut_1, aut_2;
map<int, int> aut_map;
 
bool dfs(int u, int v)
{
    if (u == v && u == 0)
        return true;
    if (u * v == 0)
        return false;
 
    bool b = true;
    if (aut_map[u] != 0 > 0 && aut_map[u] != v) 
    {
        return false;
    } else
    {
        if (aut_map[u]==v)
            return true;
        aut_map[u] = v;
        for (char c = 'a'; c<='z'; c++){
            b &= dfs(aut_1[u][c-'a'],aut_2[v][c-'a']);
        }
    }
    return b;
}
 
int main()
{
    //ofstream out("a.txt");
    aut_map = map<int, int>();
    ifstream in("isomorphism.in");
    ofstream out("isomorphism.out");
 
    int n1, m, k;
    int n2;
    in >> n1 >> m >> k;
 
    term_1 = vector<bool>(n1 + 1, false);
    int tmp;
    for (int i = 0; i < k; i++)
    {
        in >> tmp;
        term_1[tmp] = true;
    }
 
    aut_1 = vector<vector<int> >(n1 + 1, vector<int>('z' - 'a' + 1, 0) );
    int u, v;
    char c;
    for (int i = 0; i < m; i++)
    {
        in >> u >> v >> c;
        aut_1[u][c-'a'] = v;
    }
    in >> n2 >> m >> k;
    term_2 = vector<bool>(n2 + 1, false);
    for (int i = 0; i < k; i++)
    {
        in >> tmp;
        term_2[tmp] = true;
    }
 
    aut_2 = vector<vector<int> >(n2 + 1, vector<int>('z' - 'a' + 1, 0) );
    for (int i = 0; i < m; i++)
    {
        in >> u >> v >> c;
        aut_2[u][c - 'a'] = v;
    }
 
    aut_map = map<int, int>();
    bool b = dfs(1,1);
    for (int i = 1; i<=n1; i++)
        b &= !(term_1[i] ^ term_2[aut_map[i]]);
    out << (b ? "YES" : "NO");
    return 0;
}
