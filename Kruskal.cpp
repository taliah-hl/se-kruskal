// Author is Chiu CC
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>      // for setw()
#include <iostream>


struct Edge {   // Typo error. The programmer wanted to type ‘struct’ but ‘structure’.
    int from, to, weight;
    Edge() {};
    Edge(int u, int v, int w) : from(u), to(v), weight(w) {};
};

class GraphMST {
private:
    int num_vertex;
    std::vector<std::vector<int>> AdjMatrix;
public:
    GraphMST() : num_vertex(0) {};
    GraphMST(int n) : num_vertex(n) {
        AdjMatrix.resize(num_vertex);
        for (int i = 0; i < num_vertex; i++) {  // The initial value is 0 but the iterator is --, which make the for loop unable to work
            AdjMatrix[i].resize(num_vertex, 0);
        }
    }
    void AddEdge(int from, int to, int weight);
    void KruskalMST();
    void GetSortedEdge(std::vector<struct Edge>& vec);
    friend int FindSetCollapsing(int* subset, int i);
    friend void UnionSet(int* subset, int x, int y);
};

// The function FindSetCollapsing is declared to return int on line 29 but return is declared to be void type on line 32.
int FindSetCollapsing(int* subset, int i) {      // 用遞迴做collapsing
    if (subset[i] < 0) {
        return i;
    }
    return subset[i] = FindSetCollapsing(subset, subset[i]);
}

void UnionSet(int* subset, int x, int y) {
    int xroot = FindSetCollapsing(subset, x),
        yroot = FindSetCollapsing(subset, y);

    int xroot = FindSetCollapsing(subset, x), yroot = FindSetCollapsing(subset, y); // Typo error. Wanting to type ‘FindSetCollapsing’ but use ‘FinSetCollaps’.
    // also Order of parameter of FindSetCollapsing is wrong
    // 用rank比較, 負越多表示set越多element, 所以是值比較小的element比較多
    // xroot, yroot的subset[]一定都是負值
    if (subset[xroot] <= subset[yroot]) {       // x比較多element或是一樣多的時候, 以x作為root
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else {  //  if (subset[xroot] > subset[yroot]), 表示y比較多element
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}

bool WeightComp(struct Edge e1, struct Edge e2) {
    return (e1.weight < e2.weight);         // Typo corrected. There deleted redundant ‘)’.
}

void GraphMST::GetSortedEdge(std::vector<struct Edge>& edgearray) {
    for (int i = 0; i < num_vertex - 1; i++) {
        for (int j = i + 1; j < num_vertex; j++) {
            if (AdjMatrix[i][j] != 0) {
                edgearray.push_back(Edge(i, j, AdjMatrix[i][j]));   // typo corrected fro ,push_back to .push_back
            }
        }
    }
    // 用std::sort 排序, 自己定義一個comparison
    std::sort(edgearray.begin(), edgearray.end(), WeightComp);  // typo corrected: edgearray,begin() should be edgearray.begin()
}

void GraphMST::KruskalMST() {
    std::vector<struct Edge> edgesetMST;
    int* subset = new int[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        subset[i] = -1;
    }

    std::vector<struct Edge> increaseWeight;
    GetSortedEdge(increaseWeight);
    // 得到排好序的edge的vec        // symbol to comment changed from # to //.
    for (const auto& edge : increaseWeight) {
        if (FindSetCollapsing(subset, edge.from) != FindSetCollapsing(subset, edge.to)) {
            edgesetMST.push_back(edge);
            UnionSet(subset, FindSetCollapsing(subset, edge.from), FindSetCollapsing(subset, edge.to));
        }
    }

    // 印出vertex與vertex之predecessor
    int ans = 0;
    std::cout << "v1 - v2 : weight\n";
    for (const auto& edge : edgesetMST) {
        std::cout << std::setw(3) << edge.from << " - " << std::setw(3) << edge.to
            << " : " << std::setw(4) << edge.weight << "\n";
        ans += edge.weight;
    }
    // 印出MST的總weight
    std::cout << "Total weight of MST: " << ans << "\n";

    delete[] subset;
}

void GraphMST::AddEdge(int from, int to, int weight) {
    AdjMatrix[from][to] = weight;
    AdjMatrix[to][from] = weight;
}

int main() {
    GraphMST g6(7);     // kruskal is for undirected graph, so redundant edges are removed
    g6.AddEdge(0, 1, 5); g6.AddEdge(0, 5, 3);
    g6.AddEdge(1, 2, 10); g6.AddEdge(1, 4, 1); g6.AddEdge(1, 6, 4);
    g6.AddEdge(2, 3, 5); g6.AddEdge(2, 6, 8);
    g6.AddEdge(3, 4, 7); g6.AddEdge(3, 6, 9);
    g6.AddEdge(4, 5, 6); g6.AddEdge(4, 6, 2);
    g6.AddEdge(6, 4, 2);

    std::cout << "MST found by Kruskal:\n";
    g6.KruskalMST();

    return 0;
}