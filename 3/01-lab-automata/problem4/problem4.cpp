#include <fstream>
#include <vector>

static const size_t MOD = 1000000007;

using namespace std;

int main()
{
    ifstream in("problem4.in");
    ofstream out("problem4.out");
    size_t n, m, k, len;
    in >> n >> m >> k >> len;
    vector<bool> terminal(n);
    for (size_t i = 0; i < k; i++)
    {
        size_t t;
        in >> t;
        terminal[t - 1] = true;
    }
    vector<size_t> from(m);
    vector<size_t> to(m);
    for (size_t i = 0; i < m; i++)
    {
        char ch;
        in >> from[i] >> to[i] >> ch;
        from[i]--;
        to[i]--;
    }
    vector<size_t> dp(n);
    for (size_t i = 0; i < n; i++)
        dp[i] = terminal[i] ? 1 : 0;
    for (size_t i = 0; i < len; i++)
    {
        vector<size_t> new_dp(n, 0);
        for (size_t j = 0; j < m; j++)
        {
            new_dp[from[j]] += dp[to[j]];
            new_dp[from[j]] %= MOD;
        }
        dp = new_dp;
    }
    out << dp[0] << endl; 
}
