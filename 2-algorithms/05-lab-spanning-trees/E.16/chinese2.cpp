#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <map>
#include <utility>

using namespace std;

static const signed long long int inf = 1e18;

struct edge
{
    unsigned short from, to;
    long long w;
    unsigned short enabled;
    unsigned short to_old;
    edge* image;

    edge(int from, int to, long long w) : from(from), to(to), w(w), enabled(0), to_old(to) {}
};

int n;
vector<vector<edge*> > g;
int revision = 0, revision_dfs = 0;
vector<int> used;

void dfs(int v)
{
    used[v] = revision_dfs;
    for (size_t i = 0; i < g[v].size(); i++)
    {
        edge* e = g[v][i];
        if (used[e->to] != revision_dfs && e->enabled == revision)
        {
            dfs(e->to);
        }
    }
}

bool check_if_ok()
{
    revision_dfs++;
    dfs(0);
    for (int i = 0; i < n; i++)
        if (used[i] != revision_dfs)
            return false;
    return true;
}

struct finder
{
    vector<vector<edge*> >& g;
    int n;
    vector<vector<edge*> > min_edges;
    vector<int> used;
    vector<edge*> path;
    bool cycled;

    finder(vector<vector<edge*> >& g) : g(g), n(g.size()), min_edges(n), used(n, 0), cycled(false) {}

    void dfs2(int v, int color)
    {
        used[v] = color;
        for (size_t i = 0; i < min_edges[v].size() && !cycled; i++)
        {
            edge* e = min_edges[v][i];
            if (used[e->to] == color)
            {
                path.push_back(e);
                cycled = true;
                return;
            }
            if (used[e->to] == 0)
            {
                path.push_back(e);
                dfs2(e->to, color);
                if (cycled)
                    return;
                path.pop_back();
            }
        }
    }

    vector<edge*> get()
    {
        n = (int)g.size();
        vector<long long> min_w(n, inf);
        for (int i = 0; i < n; i++)
        {
            for (size_t j = 0; j < g[i].size(); j++)
            {
                edge* e = g[i][j];
                min_w[e->to] = min(min_w[e->to], e->w);
            }
        }

        {vector<bool> used_min(n, false);
        for (int i = 0; i < n; i++)
        {
            for (size_t j = 0; j < g[i].size(); j++)
            {
                edge* e = g[i][j];                
                if (e->w == min_w[e->to])
                {
                    if (used_min[e->to])
                        continue;
                    used_min[e->to] = true;
                    min_edges[e->from].push_back(e);
                }
            }
        }}

        int color = 1;
        for (int i = 0; i < n; i++)
        {
            if (used[i] == 0)
            {
                dfs2(i, color++);
                if (cycled)
                    break;
            }
        }

        if (!cycled)
        {
            vector<edge*> ans;
            for (int i = 0; i < n; i++)
                for (size_t j = 0; j < min_edges[i].size(); j++)
                    ans.push_back(min_edges[i][j]);
            return ans;
        }

        min_edges.clear();
        min_edges.resize(0);
        min_edges.reserve(0);
        min_w.clear();
        min_w.resize(0);
        min_w.reserve(0);
        used.clear();
        used.resize(0);
        used.reserve(0);

        vector<bool> on_cycle(n, false);
        for (size_t i = 0; i < path.size(); i++)
            on_cycle[path[i]->from] = true;

        vector<int> new_color(n, -1);
        int cur_color = 0;
        for (int i = 0; i < n; i++)
            if (!on_cycle[i])
                new_color[i] = cur_color++;
        for (int i = 0; i < n; i++)
            if (on_cycle[i])
                new_color[i] = cur_color;
        cur_color++;
        vector<vector<edge*> > new_g(cur_color);
        for (int i = 0; i < cur_color; i++)
            new_g[i].reserve(0);

        {
            map<pair<int, int>, edge*> cache;
            for (int i = 0; i < n; i++)
            {
                for (size_t j = 0; j < g[i].size(); j++)
                {
                    edge* e = g[i][j];
                    if (on_cycle[e->to] && on_cycle[e->from])
                        continue;

                    int from = new_color[e->from];
                    int to = new_color[e->to];
                    pair<int, int> p(from, to);
                    edge* mapped = cache[p];
                    if (mapped == 0)
                        mapped = new edge(from, to, e->w - min_w[e->to]);
                    else
                        mapped->w = min(mapped->w, e->w- min_w[e->to]);
                    mapped->to_old = e->to;
                    mapped->image = e;
                    new_g[mapped->from].push_back(mapped);
                }
            }

        }

        on_cycle.clear();
        on_cycle.resize(0);
        on_cycle.reserve(0);
        g.clear();
        g.resize(0);
        g.reserve(0);

        // =============================================
        vector<edge*> pre_ans = finder(new_g).get(); // |
        // =============================================
        int bad_end = -1;
        for (size_t i = 0; i < pre_ans.size(); i++)
        {
            edge* e = pre_ans[i];
            if (e->to == cur_color - 1)
                bad_end = e->to_old;
        }
        for (size_t i = 0; i < pre_ans.size(); i++)
        {            
            pre_ans[i] = pre_ans[i]->image;
        }
        for (size_t i = 0; i < path.size(); i++)
        {
            edge* e = path[i];
            if (e->to != bad_end)
                pre_ans.push_back(e);
        }

//        for (int i = 0; i < cur_color; i++)
//            for (size_t j = 0; j < new_g[i].size(); j++)
//                delete new_g[i][j];

        return pre_ans;
    }
};


int main()
{
    ifstream in("chinese.in");
    ofstream out("chinese.out");
    int m;
    in >> n >> m;
    g = vector<vector<edge*> >(n);
    used = vector<int>(n, 0);
    for (int i = 0; i < m; i++)
    {
        int a, b;
        long long w;
        in >> a >> b >> w;
        if (b == 1)
            continue;
        g[a - 1].push_back(new edge(a - 1, b - 1, w));
    }
    if (!check_if_ok())
    {
        out << "NO\n";
        return 0;
    }

    vector<edge*> ans = finder(g).get();
    assert((int)ans.size() == n - 1);
    long long ans2 = 0;
    for (size_t i = 0; i < ans.size(); i++)
        ans2 += ans[i]->w;
    out << "YES\n" << ans2 << "\n";
}
