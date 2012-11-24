#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <set>
 
#include <tr1/unordered_set>
#define set std::tr1::unordered_set
  
using namespace std;
  
vector<bool> term, new_term;
vector<vector<int> > aut, new_aut;
vector<vector<vector<int> > > inv_aut;
vector< set<int> > classes;
queue<int> q;
vector<bool> is_used;
vector<bool> in_queue;
set<int> temp1, temp2, reach;
vector<int> eq, bi;
vector<bool> kill;
  
void kill_unr(int v)
{
    kill[v] = true;
    for (char c = 'a'; c <= 'z'; c++)
    {
        if (!kill[aut[v][c - 'a']])
            kill_unr(aut[v][c - 'a']);
    }
}
  
void split(int num, char c)
{
    int vert, ololo, min__;
    reach = set<int>();
    map<int, int> mmm; 
    for (set<int>::iterator it = classes[num].begin(); it != classes[num].end(); it++)
    {
        vert = *it;
        for (int i = 0; i < inv_aut[vert][c - 'a'].size(); i++)
        {
            ololo = inv_aut[vert][c - 'a'][i];
            reach.insert(eq[ololo]);
            mmm[ololo]++;
        }
    }
      
    for (set<int>::iterator i = reach.begin(); i != reach.end(); i++)
    {
        ololo = *i;
        if (mmm[ololo] == classes[ololo].size())
            continue;
        temp1 = set<int>();
        temp2 = set<int>();
        for (set<int>::iterator it = classes[ololo].begin(); it != classes[ololo].end(); it++)
        {
            vert = *it;
            if (classes[num].count(aut[vert][c - 'a']) != 0)
                temp1.insert(vert);
            else
                temp2.insert(vert);
        }
        if (temp1.size() != 0 && temp2.size() != 0)
        {
            classes.push_back(temp1);
            classes.push_back(temp2);
            if (ololo % 2) classes[ololo].clear();
  
            in_queue.push_back(false);
            in_queue.push_back(false);
              
            is_used.push_back(true);
            is_used.push_back(true);
            is_used[ololo] = false;
  
            for (set<int>::iterator it1 = temp1.begin(); it1 != temp1.end(); it1++)
            {
                eq[*it1] = classes.size() - 2;
            }
  
            for (set<int>::iterator it2 = temp2.begin(); it2 != temp2.end(); it2++)
            {
                eq[*it2] = classes.size() - 1;
            }
  
            if (in_queue[ololo])
            {
                in_queue[ololo] = false;
                in_queue[classes.size() - 1] = true;
                in_queue[classes.size() - 2] = true;
                q.push(classes.size() - 1);
                q.push(classes.size() - 2);
            } else
            {
                min__ = (classes[classes.size() - 1].size() < classes[classes.size() - 2].size() ) ? (classes.size() - 1) : (classes.size() - 2);
                in_queue[min__] = true;
                q.push(min__);
            }
              
        }
    }
}
  
int main()
{
    ifstream in("fastminimization.in");
    ofstream out("fastminimization.out");
  
    int n, m, k;
    in >> n >> m >> k;
  
    int tmp;
    term = vector<bool>(n + 1, false);
    for (int i = 0; i < k; i++)
    {
        in >> tmp;
        term[tmp] = true;
    }
  
    aut = vector<vector<int> >(n + 1, vector<int>('z' - 'a' + 1, 0) );
    inv_aut = vector<vector<vector<int> > >(n + 1, vector<vector<int> > ('z' - 'a' + 1, vector<int>()));
    int u, v;
    char c;
    for (int i = 0; i < m; i++)
    {
        in >> u >> v >> c;
        aut[u][c - 'a'] = v;
        inv_aut[v][c - 'a'].push_back(u);
    }
  
    kill = vector<bool>(n + 1, false);
    kill_unr(1);
  
    for (int i = 1; i < kill.size(); i++)
        if (!kill[i])
        {
            for (char c = 'a'; c <= 'z'; c++)
                aut[i][c - 'a'] = 0;
                term[i] = false;
        }
  
    temp1 = set<int>();
    temp2 = set<int>();
      
    eq = vector<int>(n + 1);
    for (int i = 0; i <= n; i++)
        if (term[i])
        {
            temp1.insert(i);
            eq[i] = 0;
        }
        else
        {
            temp2.insert(i);
            eq[i] = 1;
        }
    
    classes.push_back(temp1);
    classes.push_back(temp2);
  
    in_queue.push_back(true);
   in_queue.push_back(true);
      
    is_used.push_back(true);
    is_used.push_back(true);
  
    if (temp1.size() < temp2.size())
    {
        q.push(0);
        in_queue[1] = false;
    } else {
        q.push(1);
        in_queue[0] = false;
    }
    //q.push(0);
    //q.push(1);
  
    int cur;
    while (!q.empty())
    {
        cur = q.front();
        q.pop();
        if (!in_queue[cur])
            continue;
          
        for (char c = 'a'; c <= 'z'; c++)
            split(cur, c);
    }
  
    int new_n = 0;
    bi = vector<int>(is_used.size(), 0);
  
    for (int i = 0; i < is_used.size(); i++)
        if (is_used[i])
        {
            new_n++;
            bi[i] = new_n;
        }
  
    new_aut = vector<vector<int> >(new_n + 1, vector<int>('z' - 'a' + 1, 0) );
    int new_m = 0;
    for (int i = 0; i < aut.size(); i++)
    {
        for (int j = 0; j < aut[i].size(); j++)
            if (aut[i][j] != 0)
            {
                new_aut[bi[eq[i]]][j] = bi[eq[aut[i][j]]];  
            }
    }
  
    new_term = vector<bool>(new_n + 1);
    int new_k = 0;
    for (int i = 0; i < term.size(); i++)
        if (term[i] && is_used[eq[i]] && !new_term[bi[eq[i]]])
        {
            new_term[bi[eq[i]]] = true;
            new_k++;
        }
  
    int hell = 0;
    for (int i = 1; i <= new_n; i++)
    {
        if (new_term[i])
            continue;
        bool good = false;
        for (int j = 0; j < new_aut[i].size(); j++)
            if (new_aut[i][j] != i && new_aut[i][j] != 0)
                good = true;
        if (!good)
            hell = i;
    }
  
    for (int i = 0; i <= new_n; i++)
        for (int j = 0; j < new_aut[i].size(); j++)
        {
            if (new_aut[i][j] == hell)
                new_aut[i][j] = 0;
            if (new_aut[i][j] == new_n)
                new_aut[i][j] = hell;
            if (new_aut[i][j] != 0)
                new_m++;
        }
    new_aut[hell] = new_aut[new_n];
    new_term[hell] = new_term[new_n];
    new_n--;
  
    int start = bi[eq[1]];
    if (start == new_n + 1)
        start = hell;
    for (int i = 1; i <= new_n; i++)
        for (int j = 0; j < new_aut[i].size(); j++)
        {
            if (new_aut[i][j] == start)
                new_aut[i][j] = 1;
            else if (new_aut[i][j] == 1)
                new_aut[i][j] = start;
        }
    swap(new_aut[start], new_aut[1]);
 
    out << new_n << ' ' << new_m << ' ' << new_k << '\n';
    for (int i = 0; i + 1 < new_term.size(); i++)
        if (new_term[i])
            out << i << ' ';
    out << '\n';
  
    for (int i = 0; i + 1 < new_aut.size(); i++)
    {
        for (int j = 0; j < new_aut[i].size(); j++)
            if (new_aut[i][j] != 0)
                out << i << ' ' << new_aut[i][j] << ' ' << static_cast<char>(j + 'a') << '\n';
    }
    return 0;
}
