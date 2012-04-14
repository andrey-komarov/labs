#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

size_t ID = 0;
size_t ALPHA;
size_t INF;

struct suffix_tree
{

    vector<int> s;

    struct edge;

    struct vertex
    {
        edge* edges[11];
        vertex* suf;
        size_t id;
        size_t depth;

        vertex(size_t depth) :
            id(ID++),
            depth(depth)
        {
            fill(edges, edges + ALPHA, (edge*)NULL);
            suf = NULL;
        }
    };

    struct edge
    {
        vertex *from, *to;
        size_t left, right;

        edge(vertex *from, size_t left) :
            from(from),
            to(new vertex(from->depth + INF - left)),
            left(left),
            right(INF)
        {}

        edge(vertex *from, vertex *to, size_t left, size_t right) :
            from(from),
            to(to),
            left(left),
            right(right)
        {}

        size_t length() const
        {
            return right - left;
        }

    };


    bool can_go(int ch)
    {
        if (current_vertex != NULL)
            return current_vertex->edges[ch] != NULL;
        if (current_edge != NULL)
            return s[current_edge->left + depth] == ch;
        assert (false);
    }

    void go(int ch)
    {
        if (current_vertex != NULL)
        {
            current_edge = current_vertex->edges[ch];
            depth = 1;
            current_vertex = NULL;
        }
        else if (current_edge != NULL)
            depth++;

        if (current_edge != NULL && current_edge->left + depth == current_edge->right)
        {
            current_vertex = current_edge->to;
            current_edge = NULL;
            depth = 0;
        }
    }

    void create_new_leaf_here(int ch)
    {
        assert (!can_go(ch));
        if (current_vertex != NULL)
            current_vertex->edges[ch] = new edge(current_vertex, current_letter);
        else if (current_edge != NULL)
        {
            vertex *new_vertex = new vertex(current_edge->from->depth + depth);

            if (last_vertex != NULL)
            {
                last_vertex->suf = new_vertex;
                last_vertex = NULL;
            }

            edge *new_edge = new edge(new_vertex, current_edge->to, current_edge->left + depth, current_edge->right);
            int old_symbol = s[new_edge->left];
            new_vertex->edges[old_symbol] = new_edge;
            new_vertex->edges[ch] = new edge(new_vertex, current_letter);
            current_edge->to = new_vertex;
            current_edge->right = current_edge->left + depth;

            current_vertex = new_vertex;
            last_edge = current_edge;
            last_depth = depth;
            current_edge = NULL;
            depth = 0;
            last_vertex = new_vertex;
        }
    }

    void jump_suffix_link()
    {
        assert (current_vertex != NULL);
        if (current_vertex->suf != NULL)
            current_vertex = current_vertex->suf;
        else
        {
            size_t need_left = last_edge->left;
            size_t need_right = need_left + last_depth;
            vertex *now = last_edge->from->suf;
            current_edge = now->edges[(int)s[need_left]];
            while (need_right - need_left > current_edge->length())
            {
                need_left += current_edge->length ();
                current_edge = current_edge->to->edges[(int)s[need_left]];
            }

            if (need_right - need_left == current_edge->length())
            {
                current_vertex = current_edge->to;
                current_edge = NULL;
                if (last_vertex != NULL)
                {
                    last_vertex->suf = current_vertex;
                    last_vertex = NULL;
                }
            }
            else
            {
                current_vertex = NULL;
                depth = need_right - need_left;
            }
        }

    }

    vertex* hell;
    vertex* root;
    vertex *current_vertex;
    edge* current_edge;
    size_t depth;
    vertex* last_vertex;
    size_t last_depth;
    edge* last_edge;
    size_t current_letter;

    void append(int ch)
    {
        while (!can_go(ch))
        {
            create_new_leaf_here(ch);
            jump_suffix_link();
        }
        go(ch);
    }

    suffix_tree(vector<int> s) :
        s(s),
        hell(new vertex(-1)),
        root(new vertex(0)),
        current_vertex(root),
        current_edge(NULL),
        last_vertex(NULL)
    {
	this->s.push_back(0);
	INF = this->s.size();

        edge *from_hell = new edge(hell, root, -1, 0);
        for (size_t i = 0; i < ALPHA; i++)
            hell->edges[i] = from_hell;
        hell->suf = hell;
        root->suf = hell;

        for (current_letter = 0; current_letter < this->s.size (); current_letter++)
            append(this->s[current_letter]);
    }

    long long refrain_size;
    size_t refrain_id;
    vector<int> refrain;
    vector<edge*> path;

    int traverse(vertex* v, size_t depth)
    {
        int size = 0;
        for (size_t i = 0; i < ALPHA; i++)
            if (v->edges[i] != NULL)
            {
                edge* e = v->edges[i];
                size += traverse(e->to, depth + e->length());
            }
	if (refrain_size < size * (long long)depth && depth != INF)
	{
            refrain_size = size * (long long)depth;
	    refrain_id = v->id;
	}
        if (size == 0)
	    size = 1;
        return size;
    }

    bool traverse2(vertex* v)
    {
        if (v->id == refrain_id)
	    return true;
	for (size_t i = 0; i < ALPHA; i++)
	{
            if (v->edges[i] != NULL)
	    {
	        edge* e = v->edges[i];
                path.push_back(e);
                if (traverse2(e->to))
	            return true;
                path.pop_back();
            }
	}
	return false;
    }

    void traverse()
    {
        refrain_size = 0;
	refrain = vector<int>();
	path = vector<edge*>();
        traverse(root, 0);
	traverse2(root);
	for (size_t i = 0; i < path.size(); i++)
	    for (size_t j = path[i]->left; j < path[i]->right; j++)
	    	refrain.push_back(s[j]);
    }
};


int main()
{
    ifstream in("refrain.in");
    ofstream out("refrain.out");
    size_t n;
    in >> n;
    in >> ALPHA;
    vector<int> s(n);
    ALPHA++;
    for (size_t i = 0; i < n; i++)
    	in >> s[i];
    suffix_tree t(s);
    t.traverse();
    if (t.refrain_size < s.size())
    {
        t.refrain_size = s.size();
	t.refrain = s;
    }
    out << t.refrain_size << "\n";
    out << t.refrain.size() << "\n";
    for (size_t i = 0; i < t.refrain.size(); i++)
    	out << t.refrain[i] << " ";
    out << "\n";
}
