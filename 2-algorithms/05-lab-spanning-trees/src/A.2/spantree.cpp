#include <fstream>
#include "math.h"

using namespace std;

struct point
{
    int x, y;
    point(){}
    point(int x, int y) : x(x), y(y) {}
};

int dist(point p1, point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int main()
{
    ifstream in("spantree.in");
    freopen("spantree.out", "w", stdout);
    size_t n;
    in >> n;
    point p[n];
    for (size_t i = 0; i < n; i++)
    {
        int x, y;
        in >> x >> y;
        p[i] = point(x, y);
    }

    const int INF = 1000000000;

    int d[n];
    bool used[n];
    for (size_t i = 0; i < n; i++)
    {
        d[i] = INF;
        used[i] = false;
    }
    d[0] = 0;

    double ans;

    while (true)
    {
        int min_pos = -1;
        int mn = INF;
        for (size_t i = 0; i < n; i++)
        {
            if (!used[i] && d[i] < mn)
            {
                mn = d[i];
                min_pos = i;
            }
        }
        if (min_pos == -1)
            break;
        ans += sqrt(mn);
        used[min_pos] = true;
        for (size_t i = 0; i < n; i++)
        {
            int w2 = dist(p[min_pos], p[i]);
            if (d[i] > w2)
                d[i] = w2;
        }
    }
    printf("%.20f\n", ans);
}
