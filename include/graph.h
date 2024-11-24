#ifndef GRAPH_H
#define GRAPH_H

typedef int vertex;
typedef int street;
typedef int region;


struct EdgeNode {
    vertex vert;
    EdgeNode* next;

    int lenght;
    int maxSpeed;
    int nBuildings;
    street street;
    region region;
};


class Graph {
private:
    int m_numVertices; //Immutable
    int m_numEdges; //Edges count will be track to easen life in some parts
    EdgeNode** m_edges; //Two references because it's a list of nodes

public:
    Graph(int numVertices);
    ~Graph();
    EdgeNode** edges();

    bool hasEdge(vertex, vertex); //O(V)
    void addEdge(vertex, vertex); //O(V)
    void removeEdge(vertex, vertex); //O(V)

    void print(); //O(V + E)

    bool isSubGraph(Graph&); //O(E + E')
    bool isValidPath(vertex[], int, bool&); //O(n * V)

    bool hasPath(vertex, vertex); //O(V + E) - envelops below function
    void findConnected(vertex, bool[]); //O(V + E)
    void dfs(int[], int[], int[]);
    void dfsRecursive(vertex, int[], int&, int[], int&, int[]);

    bool isTopological(); //theta(V + E)
    bool hasTopologicalOrder(int[]); //theta(V + E)
};

#endif