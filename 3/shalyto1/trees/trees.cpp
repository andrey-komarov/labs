#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

vector<int> l, r, p;
map<int, int> predicateToDir;
int usedV = 1;

vector<bool> isLeaf;
vector<int> leftRes;
vector<int> rightRes;
vector<int> predicate;

int dfs(int v) 
{
    if (l[v] == -1)
    {
        isLeaf.push_back(true);
        leftRes.push_back(-1);
        rightRes.push_back(-1);
        predicate.push_back(p[v]);
        return leftRes.size() - 1;
    }
    if (predicateToDir.count(p[v]))
    {
        if (predicateToDir[p[v]] == 1)
        {
            return dfs(r[v]);
        } else {
            return dfs(l[v]);
        }
    }
    int me = leftRes.size();
    isLeaf.push_back(false);
    leftRes.push_back(-1);
    rightRes.push_back(-1);
    predicate.push_back(p[v]);
    predicateToDir[p[v]] = -1;
    int newL = dfs(l[v]); 
    leftRes[me] = newL;
    predicateToDir[p[v]] = 1;
    int newR = dfs(r[v]);
    predicateToDir.erase(p[v]);
    rightRes[me] = newR;
    return me;
}

int main()
{
    ifstream in("trees.in");
    ofstream out("trees.out");
    size_t n;
    in >> n;
    l = vector<int>(n, -1);
    r = vector<int>(n, -1);
    p = vector<int>(n);
    for (size_t i = 0; i < n; i++)
    {
        string s;
        in >> s;
        if (s == "choice")
        {
            in >> p[i] >> l[i] >> r[i];
            l[i]--;
            r[i]--;
        } else {
            in >> p[i];
        }
    }
    dfs(0);
    out << isLeaf.size() << "\n";
    for (size_t i = 0; i < isLeaf.size(); i++)
    {
        if (isLeaf[i])
            out << "leaf " << predicate[i] << "\n";
        else {
            out << "choice " << predicate[i] << " " << leftRes[i]+1 << " " << rightRes[i]+1 << "\n";
        }
    }
}
