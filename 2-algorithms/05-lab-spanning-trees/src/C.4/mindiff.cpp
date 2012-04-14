#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

struct edge
{
    int a, b, w;
    bool operator<(const edge& e) const
    {   return w < e.w;}
};

int *p;

int get(int v)
{    return p[v] == v ? v : p[v] = get(p[v]);}

void unite(int v1, int v2)
{   p[get(v1)] = get(v2);}

int main()
{
    ifstream in("mindiff.in");
    ofstream out("mindiff.out");
    size_t n, m;
    in >> n >> m;
    edge e[m];
    for (size_t i = 0; i < m; i++)
    {
        in >> e[i].a >> e[i].b >> e[i].w;
        e[i].a--;
        e[i].b--;
    }
    sort(e, e + m);
    p = new int[n];

    int ans = 2100000001;
    bool ok = false;
    for (size_t start = 0; start < m; start++)
    {
        for (size_t i = 0; i < n; i++)
            p[i] = i;
        int last = 0;
        size_t cnt = 0;
        for (size_t i = start; i < m; i++)
            if (get(e[i].a) != get(e[i].b))
            {
                unite(e[i].a, e[i].b);
                last = e[i].w;
                cnt++;
            }
        if (cnt + 1 != n)
            break;
        else
            ok = true;
        ans = min(ans, last - e[start].w);
    }
    if (ok)
        out << "YES" << endl << ans << endl;
    else
        out << "NO" << endl;
}
