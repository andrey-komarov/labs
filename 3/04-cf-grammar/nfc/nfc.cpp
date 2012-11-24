#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const long long int MOD = 1000000007;

int main()
{
    ifstream in("nfc.in");
    ofstream out("nfc.out");
    int n;
    char ch;
    in >> n >> ch;
    vector<string> rules[256];
    int finishesCnt[256][256];
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            finishesCnt[i][j] = 0;
    for (int i = 0; i < n; i++)
    {
        string s, arrow, to;
        in >> s >> arrow >> to;
        if (to.size() == 1)
        {
            finishesCnt[s[0]][to[0]]++;
        } else {
            rules[s[0]].push_back(to);
        }
    }
    string s;
    in >> s;
    size_t m = s.size();
    vector<vector<vector<long long> > > dp(26, vector<vector<long long> >(m + 1, vector<long long>(m + 1, 0)));
    for (char ch = 'A'; ch <= 'Z'; ch++)
    {
        char ch2 = ch - 'A';
        for (size_t i = 0; i < m; i++)
        {
            dp[ch2][i][i+1] = finishesCnt[ch][s[i]];
        }
    }
    for (int length = 2; length <= m; length++)
    {
        for (int start = 0; start + length <= m; start++)
        {
            for (char rule = 'A'; rule <= 'Z'; rule++)
            {
                long long cnt = 0;
                for (int drop = 1; drop < length; drop++)
                {
                    for (int ruleN = 0; ruleN < rules[rule].size(); ruleN++)
                    {
                        const string& r = rules[rule][ruleN];
                        cnt += dp[r[0]-'A'][start][start + drop] * dp[r[1]-'A'][start + drop][start + length];
                        cnt %= MOD;
                    }
                }
                dp[rule-'A'][start][start + length] = cnt;
            }
        }
    }
    out << dp[ch - 'A'][0][m];
}
