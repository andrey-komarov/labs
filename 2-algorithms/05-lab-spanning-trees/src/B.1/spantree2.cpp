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
    ifstream in("spantree2.in");
    ofstream out("spantree2.out");
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
    for (int i = 0; i < n; i++)
        p[i] = i;
    int ans = 0;
    for (size_t i = 0; i < m; i++)
        if (get(e[i].a) != get(e[i].b))
        {
            unite(e[i].a, e[i].b);
            ans += e[i].w;
        }
    out << ans << "\n";
}

