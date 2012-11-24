#include <fstream>
#include <vector>
#include <bitset>
#include <map>
#include <utility>
#include <queue>

using namespace std;

static const size_t MAXN = 100;
static const size_t MOD = 1000000007;

bool comp(const bitset<MAXN>& a, const bitset<MAXN>& b)
{
    return a.to_string() < b.to_string();
}

int main()
{
    ifstream in("problem5.in");
    ofstream out("problem5.out");
    size_t n, m, k, len;
    in >> n >> m >> k >> len;
    vector<vector<vector<size_t > > > g(n, vector<vector<size_t> >(26));
    vector<bool> terminal(n);
    for (size_t i = 0; i < k; i++)
    {
        size_t t;
        in >> t;
        terminal[t - 1] = true;
    }
    for (size_t i = 0; i < m; i++)
    {
        size_t from, to;
        char ch;
        in >> from >> to >> ch;
        g[from - 1][ch - 'a'].push_back(to - 1);
    }
    map<bitset<MAXN>, size_t, bool(*)(const bitset<MAXN>&, const bitset<MAXN>&)> new_ids(comp);
    bitset<MAXN> initial;
    initial.set(0);
    new_ids[initial] = 0;
    size_t used_vertices = 1;
    queue<bitset<MAXN> > q;
    q.push(initial);
    vector<size_t> from, tos;
    vector<bool> new_terminal;
    new_terminal.push_back(terminal[0]);
    while (!q.empty())
    {
        bitset<MAXN> now = q.front();
        q.pop();
        for (size_t ch = 0; ch < 26; ch++)
        {
            bitset<MAXN> to;
            bool term = false;
            for (size_t i = 0; i < n; i++)
            {
                if (!now[i])
                    continue;
                for (size_t j = 0; j < g[i][ch].size(); j++)
                {
                    size_t t = g[i][ch][j];
                    to[t] = true;
                    term |= terminal[t];
                }
            }
            if (new_ids.count(to) == 0)
            {
                new_ids[to] = used_vertices++;
                q.push(to);
                new_terminal.push_back(term);
//                new_terminal[used_vertices - 1] = term;
            }
            from.push_back(new_ids[now]);
            tos.push_back(new_ids[to]);
        }
    }
    vector<size_t> dp(used_vertices);
    for (size_t i = 0; i < used_vertices; i++)
        dp[i] = new_terminal[i] ? 1 : 0;
    for (size_t i = 0; i < len; i++)
    {
        vector<size_t> new_dp(used_vertices);
        for (size_t j = 0; j < from.size(); j++)
        {
            new_dp[from[j]] += dp[tos[j]];
            new_dp[from[j]] %= MOD;
        }
        dp = new_dp;
    } 
    size_t i = new_ids[initial];
    size_t ans = dp[i];
    out << ans << endl;
}
