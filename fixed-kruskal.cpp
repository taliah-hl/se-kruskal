// 這分是copilot 加上我自己小修寫成的
// 看來已經正確了

#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>

struct Edge {
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
        for (int i = 0; i < num_vertex; i++) {
            AdjMatrix[i].resize(num_vertex, 0);
        }
    }
    void AddEdge(int from, int to, int weight);
    void KruskalMST();
    void GetSortedEdge(std::vector<struct Edge>& vec);
    friend int FindSetCollapsing(int* subset, int i);
    friend void UnionSet(int* subset, int x, int y);
};

int FindSetCollapsing(int* subset, int i) {
    if (subset[i] < 0) {
        return i;
    }
    return subset[i] = FindSetCollapsing(subset, subset[i]);
}

void UnionSet(int* subset, int x, int y) {
    int xroot = FindSetCollapsing(subset, x),
        yroot = FindSetCollapsing(subset, y);

    if (subset[xroot] <= subset[yroot]) {
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    }
    else {
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}

bool WeightComp(struct Edge e1, struct Edge e2) {
    return (e1.weight < e2.weight);
}

void GraphMST::GetSortedEdge(std::vector<struct Edge>& edgearray) {
    for (int i = 0; i < num_vertex - 1; i++) {
        for (int j = i + 1; j < num_vertex; j++) {
            if (AdjMatrix[i][j] != 0) {
                edgearray.push_back(Edge(i, j, AdjMatrix[i][j]));
            }
        }
    }
    std::sort(edgearray.begin(), edgearray.end(), WeightComp);
}

void GraphMST::KruskalMST() {
    std::vector<struct Edge> edgesetMST;
    int* subset = new int[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        subset[i] = -1;
    }

    std::vector<struct Edge> increaseWeight;
    GetSortedEdge(increaseWeight);
    
    for (const auto& edge : increaseWeight) {
        if (FindSetCollapsing(subset, edge.from) != FindSetCollapsing(subset, edge.to)) {
            edgesetMST.push_back(edge);
            UnionSet(subset, FindSetCollapsing(subset, edge.from), FindSetCollapsing(subset, edge.to));
        }
    }

    int ans = 0;
    std::cout << "v1 - v2 : weight\n";
    for (const auto& edge : edgesetMST) {
        std::cout << std::setw(3) << edge.from << " - " << std::setw(3) << edge.to
            << " : " << std::setw(4) << edge.weight << "\n";
        ans += edge.weight;
    }
    cout << "Total weight of MST: " << ans << "\n";

    delete[] subset;
}

void GraphMST::AddEdge(int from, int to, int weight) {
    AdjMatrix[from][to] = weight;
    AdjMatrix[to][from] = weight;
}

int main() {
    GraphMST g6(7);
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