# 題IV. 比較Chat GPT審查與手動審查

**chat gpt 版本**
- 原版chat gpt 3.5


## 比較方法
- 我們設計了3個prompt，每個prompt都請求chat gpt審查code 及撰寫review 報告
- 在code review 方面，我們希望比較chat gpt 在3個不同prompt之下bug fix, refactor 及加上適切comment 的能力
- 在report 方面，我們希望chat gpt 跟我們手動review 使用相同的fault type classification (IEEE Standard 1044-2009, Standard Classification for Software Anomalies,IEEE Computer Society Press, 2009)，希望看看chat gpt 有沒有能力在給定的fault type清單中辨認fault type，以及整理成報告

## Code Review 部分



### Prompt 1
原始連結參見: https://chatgpt.com/share/0f89586c-7781-4bce-b6bd-6d3ebac5e409
prompt
```
please do code review on the following code, do 2 tasks:
1. fix the code
2. summarize what you fixed, classify the fault types / type of improvement for each enhacement you made into the following types:
    [所有fault type 都寫在這裡]

    [原整的code都寫在這裡]
```

chat gpt 產生的code
```cpp
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

```
### Prompt 2
原始連結參見: https://chatgpt.com/share/70ab705c-aeab-4f2c-bc82-3f7df7bd26a9
### prompt
```
do code review in the following manner

1. enhance the code by correcting the bugs, refactoring and adding appropriate comments
2. summarize the improvement you made in table format in .md, follow this format
| number | location (line number)| severity | fault type | brief description |

for severity, pick from: critical,severe, moderate, minor

for fault types, pick from the following list only, do not write terms that does not appear in this list in the column fault type
fault types:
    
    [所有fault type 都寫在這裡]
    

code to review:
   
   [原整的code都寫在這裡]
```

#### chat gpt 產生的code
```cpp
//prompt2
// Author is Chiu CC

#include <vector>
#include <list>
#include <iomanip>      // for setw()
#include <iostream>     // for std::cout
#include <algorithm>    // for std::sort

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
    friend int FindSetCollapsing(int* subset, int i);
    friend void UnionSet(int* subset, int x, int y);
};

int FindSetCollapsing(int* subset, int i) { // 用遞迴做collapsing
    int root;  // root
    for (root = i; subset[root] >= 0; root = subset[root]);

    while (i != root) {
        int parent = subset[i];
        subset[i] = root;
        i = parent;
    }

    return root;
}

void UnionSet(int* subset, int x, int y) {
    int xroot = FindSetCollapsing(subset, x);
    int yroot = FindSetCollapsing(subset, y);

    // 用rank比較, 負越多表示set越多element, 所以是值比較小的element比較多
    // xroot, yroot的subset[]一定都是負值
    if (subset[xroot] <= subset[yroot]) { // x比較多element或是一樣多的時候, 以x作為root
        subset[xroot] += subset[yroot];
        subset[yroot] = xroot;
    } else { //  if (subset[xroot] > subset[yroot]), 表示y比較多element
        subset[yroot] += subset[xroot];
        subset[xroot] = yroot;
    }
}

bool WeightComp(const Edge& e1, const Edge& e2) {
    return (e1.weight < e2.weight);
}

void GraphMST::GetSortedEdge(std::vector<Edge>& edgearray) {
    for (int i = 0; i < num_vertex - 1; i++) {
        for (int j = i + 1; j < num_vertex; j++) {
            if (AdjMatrix[i][j] != 0) {
                edgearray.push_back(Edge(i, j, AdjMatrix[i][j]));
            }
        }
    }
    // 用std::sort 排序, 自己定義一個comparison
    std::sort(edgearray.begin(), edgearray.end(), WeightComp);
}

void GraphMST::KruskalMST() {
    Edge* edgesetMST = new Edge[num_vertex - 1];
    int edgesetcount = 0;

    int* subset = new int[num_vertex];
    for (int i = 0; i < num_vertex; i++) {
        subset[i] = -1;
    }

    std::vector<Edge> increaseWeight;
    GetSortedEdge(increaseWeight); // 得到排好序的edge的vec

    for (int i = 0; i < increaseWeight.size(); i++) {
        if (FindSetCollapsing(subset, increaseWeight[i].from) != FindSetCollapsing(subset, increaseWeight[i].to)) {
            edgesetMST[edgesetcount++] = increaseWeight[i];
            UnionSet(subset, increaseWeight[i].from, increaseWeight[i].to);
        }
    }
    // 以下僅僅是印出vertex與vertex之predecessor
    std::cout << std::setw(3) << "v1" << " - " << std::setw(3) << "v2" << " : weight\n";
    for (int i = 0; i < num_vertex - 1; i++) {
        std::cout << std::setw(3) << edgesetMST[i].from << " - " << std::setw(3) << edgesetMST[i].to
                  << " : " << std::setw(4) << edgesetMST[i].weight << "\n";
    }

    delete[] edgesetMST;
    delete[] subset;
}

void GraphMST::AddEdge(int from, int to, int weight) {
    AdjMatrix[from][to] = weight;
    AdjMatrix[to][from] = weight; // Assuming undirected graph
}

int main() {
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


```
### Prompt 3
原始連結參見: 
prompt
```
```

chat gpt 產生的code
```
```
