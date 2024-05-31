// prompt 1
// Author is Chiu CC


#include <vector>
#include <list>
#include <iomanip>      // for setw()
#include <iostream>     // for cout
#include <algorithm>    // for sort

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
    void GetSortedEdge(std::vector<Edge> &vec);
    friend int FindSetCollapsing(int *subset, int i);
    friend void UnionSet(int *subset, int x, int y);
};
int FindSetCollapsing(int *subset, int i) {  // Function should return int
    int root;
    for (root = i; subset[root] >= 0; root = subset[root]);

    while (i != root) {
        int parent = subset[i];  // Corrected assignment operator
        subset[i] = root;
        i = parent;
    }

    return root;  // Added missing return statement
}
void UnionSet(int *subset, int x, int y) {
    int xroot = FindSetCollapsing(subset, x),  // Corrected function arguments
        yroot = FindSetCollapsing(subset, y);

    if (subset[xroot] <= subset[yroot]) {
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    } else {
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}
bool WeightComp(Edge e1, Edge e2) {
    return (e1.weight < e2.weight);  // Removed extra parenthesis
}
void GraphMST::GetSortedEdge(std::vector<Edge> &edgearray) {
    for (int i = 0; i < num_vertex; i++) {  // Changed the loop to correct iteration
        for (int j = i + 1; j < num_vertex; j++) {
            if (AdjMatrix[i][j] != 0) {  // Corrected the indices
                edgearray.push_back(Edge(i, j, AdjMatrix[i][j]));  // Corrected push_back
            }
        }
    }
    std::sort(edgearray.begin(), edgearray.end(), WeightComp);  // Fixed sort syntax
}
void GraphMST::KruskalMST() {
    Edge *edgesetMST = new Edge[num_vertex - 1];
    int edgesetcount = 0;

    int *subset = new int[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        subset[i] = -1;
    }

    std::vector<Edge> increaseWeight;
    GetSortedEdge(increaseWeight);

    for (size_t i = 0; i < increaseWeight.size(); i++) {  // Corrected size_t type
        if (FindSetCollapsing(subset, increaseWeight[i].from) != FindSetCollapsing(subset, increaseWeight[i].to)) {
            edgesetMST[edgesetcount++] = increaseWeight[i];
            UnionSet(subset, increaseWeight[i].from, increaseWeight[i].to);
        }
    }

    std::cout << std::setw(3) << "v1" << " - " << std::setw(3) << "v2" << " : weight\n";
    for (int i = 0; i < edgesetcount; i++) {  // Corrected loop to edgesetcount
        std::cout << std::setw(3) << edgesetMST[i].from << " - " << std::setw(3) << edgesetMST[i].to
                  << " : " << std::setw(4) << edgesetMST[i].weight << "\n";
    }

    delete[] edgesetMST;  // Added cleanup for dynamically allocated memory
    delete[] subset;  // Added cleanup for dynamically allocated memory
}
void GraphMST::AddEdge(int from, int to, int weight) {  // Changed char to int
    AdjMatrix[from][to] = weight;  // Corrected assignment
}

int main() {
    GraphMST g6(7);
    g6.AddEdge(0, 1, 5); g6.AddEdge(0, 5, 3);
    g6.AddEdge(1, 0, 5); g6.AddEdge(1, 2, 10); g6.AddEdge(1, 4, 1); g6.AddEdge(1, 6, 4);
    g6.AddEdge(2, 1, 10); g6.AddEdge(2, 3, 5); g6.AddEdge(2, 6, 8);
    g6.AddEdge(3, 2, 5); g6.AddEdge(3, 4, 7); g6.AddEdge(3, 6, 9);
    g6.AddEdge(4, 1, 1); g6.AddEdge(4, 3, 7); g6.AddEdge(4, 5, 6); g6.AddEdge(4, 6, 2);
    g6.AddEdge(5, 0, 3); g6.AddEdge(5, 4, 6);
    g6.AddEdge(6, 1, 4); g6.AddEdge(6, 2, 8); g6.AddEdge(6, 3, 9); g6.AddEdge(6, 4, 2);

    std::cout << "MST found by Kruskal:\n";
    g6.KruskalMST();

    return 0;
}
