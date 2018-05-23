/**
 * Made by Josh
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <assert.h>
#include <stdexcept>
#include <queue>
#include <stack>

class Nod {
public:
    int key;
    int startTime;
    int endTime;
    std::vector<int> adjacent;

    explicit Nod(int key) {
        this->key = key;
        this->startTime = -1;
        this->endTime = -1;
    }

    Nod() = default;

    void addNode(const int &nod) {
        this->adjacent.push_back(nod);
    }

    bool containNod(int nod) {
        return std::find(adjacent.begin(), adjacent.end(), nod) != adjacent.end() || this->key == nod;
    }

    void print() {

        printf("Key: %d: ", this->key);
        for (auto &i : adjacent) {
            printf("%d ", i);
        }
        printf("\n");
    }
};


class Graph {
public:
    int edgeCount;
    int vertexCount;
    std::vector<Nod> nodList;

    explicit Graph() {
        this->edgeCount = 0;
        this->vertexCount = 0;
    }

    Graph(int vertexCount) : vertexCount(vertexCount) {
        this->edgeCount = 0;
        nodList.clear();
        for (int count = 0; count < vertexCount; count++) {
            this->nodList.push_back(Nod(count));
        }
    }

    Graph(int e, int v) : edgeCount(e), vertexCount(v) {}

    void generate(int edgeCount, int vertexCount) { // generate a random graph
        this->nodList.clear();
        this->edgeCount = edgeCount;
        this->vertexCount = vertexCount;
        //create a nod for each vertex
        for (int count = 0; count < vertexCount; count++) {
            this->nodList.push_back(Nod(count));
        }

        int s, d;
        //generate edgeCount different edges
        for (int count = 0; count < edgeCount; count++) {
            s = rand() % vertexCount;
            d = rand() % vertexCount;
            while (nodList[s].containNod(d)) {
                s = rand() % vertexCount;
                d = rand() % vertexCount;
            }
            nodList[s].addNode(d);
        }
    }

    void dfs(int key) {
        assert(key < vertexCount);
        bool visited[vertexCount] = {false};
        printf("DFS:\n");
        dfsAlgorithm(key, visited);
        printf("\n");
    }

    void addEdge(int s, int d) {
        if (!this->nodList[s].containNod(d)) {
            this->nodList[s].addNode(d);
            this->edgeCount++;
        }
    }

    std::vector<Nod> topologicalSort() {
        int time = 1;
        bool hasCycle = false;
        std::vector<Nod> list;
        for (auto &i : nodList) {
            if (i.startTime == -1) {
                topologicalAlgorithm(i.key, &time, &list, &hasCycle);
            }
        }

        if (hasCycle) {
            list.clear();
        }
        return list;

    }

    void tarjan() {
        int time = 0;
        int discovered[this->vertexCount];
        int low[this->vertexCount];
        for (int i = 0; i < vertexCount; ++i) {
            discovered[i] = -1;
            low[i] = -1;
        }
        std::vector<Nod> vector;
        for (auto &i : this->nodList) {
            if (discovered[i.key] == -1) {
                tarjanAlgorithm(i.key, discovered, low, &vector, &time);
            }
        }
        printf("\n");
        int count = 0; // count for the connected components
        int i = 0;
        while (i < vector.size()) {
            printf("Component:%d: %d ", count, vector[i].key);
            while (low[i] == low[i + 1]) {
                i++;
                printf("%d ", vector[i].key);
            }
            i++;
            ++count;
            printf("\n");
        }

    }

    void print() {
        printf("Adjacent list:\n");
        for (auto &i : nodList) {
            i.print();
        }
    }

private:
    void tarjanAlgorithm(int key, int *discovered, int *low, std::vector<Nod> *vector, int *time) {
        discovered[key] = *time;
        low[key] = *time;
        (*time)++;
        for (auto &i : nodList[key].adjacent) {
            if (discovered[i] == -1) {
                tarjanAlgorithm(i, discovered, low, vector, time);
                low[key] = std::min(low[key], low[i]);
            } else {
                low[key] = std::min(discovered[i], low[key]);
            }
        }
        (*vector).push_back(nodList[key]);
    }

    void topologicalAlgorithm(int key, int *time, std::vector<Nod> *list, bool *hasCycle) {
        nodList[key].startTime = *time;
        (*time) = (*time) + 1;
        for (auto &i : nodList[key].adjacent) {
            if (nodList[i].startTime == -1) {
                topologicalAlgorithm(i, time, list, hasCycle);
            } else if (nodList[i].startTime != -1 && nodList[i].endTime == -1) {
                (*hasCycle) = true;
            }
        }
        nodList[key].endTime = (*time);
        (*time) = (*time) + 1;
        list->push_back(nodList[key]);
    }

    void dfsAlgorithm(int key, bool *visited) {
        visited[key] = true;
        printf("%d ", key);
        for (auto &i: this->nodList[key].adjacent) {
            if (!visited[i]) {
                dfsAlgorithm(i, visited);
            }
        }
    }

};

void demo() {
    Graph g;
    g.generate(3, 4);
    g.print();
    g.dfs(0);

    std::vector<Nod> vector = g.topologicalSort();

    if (vector.empty()) {
        printf("The graph has cycle and can't be applied a topological sort");
    } else {
        printf("Topological sort:\n");
        for (auto &i : vector) {
            printf("Key: %d Start: %d End:%d\n", i.key, i.startTime, i.endTime);
        }
    }

    //demo for tarjan algorithm
    printf("\nTest tarjan algorithm\n");
    Graph g1(6);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 0);
    g1.addEdge(2, 5);
    g1.addEdge(5, 4);
    g1.addEdge(4, 3);
    g1.addEdge(3, 5);
    g1.print();
    g1.tarjan();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    demo();

    return 0;
}