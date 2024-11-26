#include <string>
using namespace std;

#ifndef GRAPH_H
#define GRAPH_H

typedef int vertex;
typedef int street;
typedef int region;

typedef struct EdgeNode {
    EdgeNode* next;
    vertex endVertex;

    //important values for the crossing
    float lenght;
    float maxSpeed;
    float escavationCost;

    //building counts (should sum up to a value (function of the lenght))
    int nResidential;
    int nComercial;
    int nIndustrial;
    int nTouristic;

    //ownership infos
    string street;
    int streetOffset;
    string region;
} StreetSegment;

typedef struct VertexNode {
    int region;
    StreetSegment* segments;
} Crossing;


//Internal names and methods of the class will keep graph naming. public things will have city nomenclature
//This implementation is based on a list of adjacencies.
class CityGraph {
private:
    int m_numVertices; //Immutable - how many crosses the city has
    int m_numEdges; //Edges count will be track to easen life in some parts
    Crossing** m_vertices; //Two references because it's a list of nodes

public:
    //Class constructor and destructor
    CityGraph(int numVertex);
    ~CityGraph();

    Crossing** crossings();
    StreetSegment* m_edges(vertex);

    bool hasSegment(vertex, vertex); //O(V)
    void addSegment(vertex, vertex, EdgeNode*); //O(V)
    void removeSegment(vertex, vertex); //O(V)

    void print(); //O(V + E)

    bool isSubGraph(CityGraph&); //O(E + E')
    bool isValidPath(vertex[], int, bool&); //O(n * V)

    bool hasPath(vertex, vertex); //O(V + E) - envelops below function
    void findConnected(vertex, bool[]); //O(V + E)
    void dfs(int[], int[], int[]);
    void dfsRecursive(vertex, int[], int&, int[], int&, int[]);

    bool isTopological(); //theta(V + E)
    bool hasTopologicalOrder(int[]); //theta(V + E)

    template <typename T> void CPTDijkstra(vertex, vertex[], T[]);
};


#endif
