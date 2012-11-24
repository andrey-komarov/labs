#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

static const size_t MOD = 1000000007;
vector<vector<size_t> > g;
vector<vector<size_t> > rev_g;
vector<size_t> order;

vector<size_t> used;
vector<size_t> size;
vector<bool> terminal;
vector<bool> loop;
vector<size_t> color;
vector<bool> reachable;
size_t colors;

size_t dfs(size_t v)
{
    if (used[v] == 1)
        return 0;
    if (used[v] == 2)
        return size[v];
    used[v] = 1;
    size[v] = 0;
    for (size_t i = 0; i < g[v].size(); i++)
    {
        size_t to = g[v][i];
        size[v] += dfs(to);
        size[v] %= MOD;
    }
    used[v] = 2;
    if (terminal[v])
        size[v]++;
    size[v] %= MOD;
    return size[v];
}

void dfs_topological_sort(size_t v)
{
    used[v] = 3;
    reachable[v] = true;
    for (size_t i = 0; i < g[v].size(); i++)
    {
        size_t to = g[v][i];
        if (used[to] != 3)
            dfs_topological_sort(to);
    }
    order.push_back(v);
}

void dfs_strong_connectivity(size_t v, size_t current_color)
{
    if (!reachable[v])
        return;
    color[v] = current_color;
    used[v] = 4;
    for (size_t i = 0; i < rev_g[v].size(); i++)
    {
        size_t to = rev_g[v][i];
        if (used[to] != 4)
            dfs_strong_connectivity(to, current_color);
    }
}

int main()
{
    ifstream in("problem3.in");
    ofstream out("problem3.out");
    size_t n, m, k;
    in >> n >> m >> k;
    terminal = vector<bool>(n);
    used = vector<size_t>(n);
    size = vector<size_t>(n);
    loop = vector<bool>(n);
    color = vector<size_t>(n);
    reachable = vector<bool>(n);
    colors = 0;
    for (size_t i = 0; i < k; i++)
    {
        size_t t;
        in >> t;
        terminal[t - 1] = true;
    }
    g = vector<vector<size_t> >(n);
    rev_g = vector<vector<size_t> >(n);
    for (size_t i = 0; i < m; i++)
    {
        size_t from, to;
        char ch;
        in >> from >> to >> ch;
        g[from - 1].push_back(to - 1);
        rev_g[to - 1].push_back(from - 1);
        if (from == to)
            loop[from - 1] = true;
    }
    order = vector<size_t>();
    dfs_topological_sort(0);
    reverse(order.begin(), order.end());
    for (size_t i = 0; i < order.size(); i++)
    {
        size_t v = order[i];
        if (used[v] != 4)
            dfs_strong_connectivity(v, colors++);
    }
    vector<size_t> color_count(colors);
    for (size_t i = 0; i < order.size(); i++)
    {
        size_t v = order[i];
        color_count[color[v]]++;
    }
    queue<size_t> q;
    for (size_t i = 0; i < order.size(); i++)
    {
        size_t v = order[i];
        if (color_count[color[v]] > 1 || loop[v])
        {
            q.push(v);
            used[v] = 5;
        }
    }
    while (q.size() > 0)
    {
        size_t now = q.front();
        if (terminal[now])
        {
            out << "-1" << endl;
            return 0;
        }
        q.pop();
        for (size_t i = 0; i < g[now].size(); i++)
        {
            size_t to = g[now][i];
            if (used[to] != 5)
            {
                q.push(to);
                used[to] = 5;
            }
        }
    }
    size_t ans = dfs(0);
    out << ans << endl;
}
