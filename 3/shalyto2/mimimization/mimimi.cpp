#include <iostream>
#include <vector>

using namespace std;

static const int SEGS = 20;
static const int ITS = 980;

int n;

double query(int index, double x)
{
    for (int i = 0; i < n; i++)
    {
        if (i == index)
            cout << x << " ";
        else
            cout << "0 ";
    }
    cout << "\n";
    cout.flush();
    double f;
    cin >> f;
    return f;
}

int main()
{
    cin >> n;
    vector<double> ans;
    for (int i = 0; i < n; i++)
    {
        double best = 1000;
        double bestPos = 0;
        for (int s = 0; s < SEGS; s++)
        {
            double left = 1. / SEGS * s;
            double right  = 1. / SEGS * (s + 1);
            for (int it = 0; it < ITS / SEGS / 2; it++)
            {
                double c1 = (2 * left + right) / 3;
                double c2 = (left + 2 * right) / 3;
                double f1 = query(i, c1);
                double f2 = query(i, c2);
                if (f1 < best)
                {
                    best = f1;
                    bestPos = left;
                }
                if (f1 < f2)
                    right = c2;
                else
                    left = c1;
            }
        }
        ans.push_back(bestPos);
    }
    for (int i = 0; i < n; i++)
        cout << ans[i] << " ";
    cout << "\n";
    cout.flush();
    double f;
    cin >> f;
    cout << "minimum " << f << "\n";
    cout.flush();
}
