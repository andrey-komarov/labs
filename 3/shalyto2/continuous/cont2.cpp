#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

static const double EPS = 1e-9;
int compare(double a, double b)
{
    if (fabs(a - b) < EPS)
        return 0;
    return a < b ? -1 : 1;
}

vector<double> gauss(vector<vector<double> > a, vector<double> b)
{
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < a[i].size(); j++)
            cerr << a[i][j] << " ";
        cerr << " = " << b[i] << "\n";
    }
    size_t n = b.size();
    size_t m = a[0].size();
    for (int j = 0; j < m; j++)
    {
        int nonZero = j;
        for (int i = j; i < n; i++)
        {
            if (compare(a[i][j], 0) != 0)
            {
                nonZero = i;
                break;
            }
        }
        swap(a[j], a[nonZero]);
        swap(b[j], b[nonZero]);
        if (compare(a[j][j], 0) == 0)
            continue;
        for (int k = 0; k < m; k++)
        {
            if (k == j)
                continue;
            for (int i = 0; i < n; i++)
            {
                if (i == j)
                    continue;
                a[i][k] -= a[i][j] / a[j][j] * a[j][k];
            }
        }
        for (int i = 0; i < n; i++)
            if (i != j)
                b[i] -= a[i][j] / a[j][j] * b[j];
        for (int i = 0; i < n; i++)
            if (i != j)
                a[i][j] = 0;

    }
    for (int i = 0; i < m; i++)
        if (compare(a[i][i], 0) ==0)
        {
            b[i] = 0;
        } else {
            b[i] /= a[i][i];
        }
    return b;
}

int main()
{
    ifstream in("continuous.in");
    ofstream out("continuous.out");
    int n, m;
    in >> n >> m;
    vector<vector<int> > go(n, vector<int>(2, 0));
    vector<vector<double> > sum(2 * n, vector<double>(2 * n, 0));
    vector<double> rhs(2 * n, 0);
    for (int i = 0; i < n; i++)
    {
        in >> go[i][0] >> go[i][1];
        go[i][0]--;
        go[i][1]--;
    }
    for (int i = 0; i < m; i++)
    {
        int me = 0;
        int len;
        string inf;
        in >> len >> inf;
        vector<double> path(2 * n, 0);
        for (int j = 0; j < len; j++)
        {
            double now;
            in >> now;
            path[2 * me + inf[j] - '0'] += 1. / len;
            me = go[me][inf[j] - '0'];

            for (int j = 0; j < 2 * n; j++)
            {
                for (int k = 0; k < 2 * n; k++)
                {
                    sum[j][k] += path[j] * path[k] * len;
                    sum[k][j] += path[j] * path[k] * len;
                }
                rhs[j] += 2 * path[j] * now;
            }
        }
    }
    
    vector<double> ans = gauss(sum, rhs);

    out.precision(20);

    for (int i = 0; i < n; i++)
    {
        out << ans[2 * i] << " " << ans[2 * i + 1] << "\n";
    }
}
