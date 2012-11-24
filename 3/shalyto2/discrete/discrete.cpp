#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    ifstream in("discrete.in");
    ofstream out("discrete.out");
    int n, m;
    in >> n >> m;
    vector<vector<int> > go(n, vector<int>(2, 0));
    for (int i = 0; i < n; i++)
    {
        in >> go[i][0] >> go[i][1];
        go[i][0]--;
        go[i][1]--;
    }
    vector<vector<vector<double> > > o(n, vector<vector<double> >(2, vector<double>(26, 0)));
    for (int i = 0; i < m; i++)
    {
        int me = 0;
        int len;
        string inf, outf;
        in >> len >> inf >> outf;
        for (int j = 0; j < len; j++)
        {
            o[me][inf[j] - '0'][outf[j] - 'a'] += 1. / len;
            me = go[me][inf[j] - '0'];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            double mx = 0;
            int argMx = 0;
            for (int k = 0; k < 26; k++)
            {
                if (o[i][j][k] > mx)
                {
                    mx = o[i][j][k];
                    argMx = k;
                }
            }
            out << (char)('a' + argMx) << " ";
        }
        out << "\n";
    }
}
