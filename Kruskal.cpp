// Author is Chiu CC
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> // for setw()

struct Edge
{ // Typo error. The programmer wanted to type ‘struct’ but ‘structure’.
    int from, to, weight;
    Edge(){};
    Edge(int u, int v, int w) : from(u), to(v), weight(w){};
}; // Missing ;

class GraphMST
{
private: // Typo error. The programmer wanted to use ‘:’ but use ‘;’
    int num_vertex;
    std::vector<std::vector<int>> AdjMatrix;

public:
    GraphMST() : num_vertex(0){};
    GraphMST(int n) : num_vertex(n)
    {
        AdjMatrix.resize(num_vertex);
        for (int i = 0; i < num_vertex; i--)
        { // The initial value is 0 but the iterator is --, which make the for loop unable to work
            AdjMatrix[i].resize(num_vertex);
        }
    }
    void AddEdge(int from, int to, int weight);

    void KruskalMST();
    void GetSortedEdge(std::vector<struct Edge> &vec);
    friend int FindSetCollapsing(int *subset, int i);
    friend void UnionSet(int *subset, int x, int y);
};
int FindSetCollapsing(int *subset, int i)
{                                                          // The function FindSetCollapsing is declared to return int on line 29 but return is declared to be void type on line 32.
                                                           // 用遞迴做collapsing
    int root;                                              // root
    for (root = i; subset[root] >= 0; root = subset[root]) // The for loop can only do no-op
        ;

    while (i != root)
    {
        int parent = subset[i]; // Wanting to assign parent as subset[i], but use ‘==’
        subset[i] = root;
        i = parent;
    }
    return root;
}

void UnionSet(int *subset, int x, int y)
{

    int xroot = FindSetCollapsing(subset, x), yroot = FindSetCollapsing(subset, y); // Typo error. Wanting to type ‘FindSetCollapsing’ but use ‘FinSetCollaps’.
    // also Order of parameter of FindSetCollapsing is wrong
    // 用rank比較, 負越多表示set越多element, 所以是值比較小的element比較多
    // xroot, yroot的subset[]一定都是負值
    if (subset[xroot] <= subset[yroot])
    { // x比較多element或是一樣多的時候, 以x作為root
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else
    { //  if (subset[xroot] > subset[yroot]), 表示y比較多element
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot; // Typo error. Wanting to type ‘yroot’ but use ‘yrooot’.
    }
}

bool WeightComp(struct Edge e1, struct Edge e2)
{
    return (e1.weight < e2.weight); // Typo error. There is a redundant ‘)’.
}

void GraphMST::GetSortedEdge(std::vector<struct Edge> &edgearray)
{
    for (int i = 0; i < num_vertex - 1; i++)
    {
        for (int j = i + 1; j < num_vertex; j++)
        {
            if (AdjMatrix[i][j] != 0)
            {
                edgearray.push_back(Edge(i, j, AdjMatrix[i][j])); // edgearray,push_back should be edgearray.push_back
            }
        }
    }
    // 用std::sort 排序, 自己定義一個comparison
    std::sort(edgearray.begin(), edgearray.end(), WeightComp); // edgearray,begin() should be edgearray.begin()
}

void GraphMST::KruskalMST()
{
    struct Edge *edgesetMST = new struct Edge[num_vertex - 1];
    int edgesetcount = 0;

    int *subset = new int[num_vertex]; // Expression must be a constant value.
    for (int i = 0; i < num_vertex; i++)
    {
        subset[i] = -1;
    }

    std::vector<struct Edge> increaseWeight;
    GetSortedEdge(increaseWeight);
    // 得到排好序的edge的vec        // Use # but not // to comment in C++.

    for (int i = 0; i < increaseWeight.size(); i++)
    { // Indent where indentation is not needed
        if (FindSetCollapsing(subset, increaseWeight[i].from) != FindSetCollapsing(subset, increaseWeight[i].to))
        {
            edgesetMST[edgesetcount++] = increaseWeight[i];
            UnionSet(subset, increaseWeight[i].from, increaseWeight[i].to);
        }
    }
    // 以下僅僅是印出vertex與vertex之predecessor
    std::cout << std::setw(3) << "v1" << " - " << std::setw(3) << "v2 :weight\n"; // A string literal in your C++ code is not properly enclosed in quotation marks, resulting in a syntax error
    for (int i = 0; i < num_vertex - 1; i++)
    {
        std::cout << std::setw(3) << edgesetMST[i].from << " - " << std::setw(3) << edgesetMST[i].to
                  << " : " << std::setw(4) << edgesetMST[i].weight << "\n";
    }
}
void GraphMST::AddEdge(int from, int to, int weight)
{ // The declaration on line 25 of AddEdge parameter is not the same with this declaration.
    AdjMatrix[from][to] = weight;
    AdjMatrix[from][to] = weight;
}

int main()
{

    GraphMST g6(7);
    g6.AddEdge(0, 1, 5);
    g6.AddEdge(0, 5, 3);
    g6.AddEdge(1, 0, 5);
    g6.AddEdge(1, 2, 10);
    g6.AddEdge(1, 4, 1);
    g6.AddEdge(1, 6, 4);
    g6.AddEdge(2, 1, 10);
    g6.AddEdge(2, 3, 5);
    g6.AddEdge(2, 6, 8);
    g6.AddEdge(3, 2, 5);
    g6.AddEdge(3, 4, 7);
    g6.AddEdge(3, 6, 9);
    g6.AddEdge(4, 1, 1);
    g6.AddEdge(4, 3, 7);
    g6.AddEdge(4, 5, 6);
    g6.AddEdge(4, 6, 2);
    g6.AddEdge(5, 0, 3);
    g6.AddEdge(5, 4, 6);
    g6.AddEdge(6, 1, 4);
    g6.AddEdge(6, 2, 8);
    g6.AddEdge(6, 3, 9);
    g6.AddEdge(6, 4, 2);

    std::cout << "MST found by Kruskal:\n";
    g6.KruskalMST();
}