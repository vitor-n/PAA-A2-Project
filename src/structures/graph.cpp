#include <iostream>
#include "graph.h"
#include "linkedlist.h"
#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;

typedef LinkedList Regions;

StreetSegment* CityGraph::m_edges(vertex v){
    if(v >= 0 && v < m_numVertices){
        return m_vertices[v]->segments;
    }
    return nullptr;
}

CityGraph::CityGraph(int numVertices, int numRegions):
m_numVertices(numVertices), m_numRegions(numRegions), m_numEdges(0), m_vertices(nullptr){
    m_vertices = new VertexNode*[numVertices+2]; //Initializes the list of edges for each vetex
    regions = new Regions*[numRegions];
    for (vertex i = 0; i < numVertices+2; i++) {
        m_vertices[i] = new VertexNode;
        m_vertices[i]->region = -1;
        m_vertices[i]->segments = nullptr;
    }
    for (int i = 0; i < numRegions; i++) {
        regions[i] = new Regions(i);
    }
}

CityGraph::~CityGraph(){
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* node = m_edges(i); //Get the list head of edges for that vertex
        while (node != nullptr) { //And goes deleting everything
            EdgeNode* nextNode = node->next;
            delete node;
            node = nextNode;
        }
        delete m_vertices[i];
    }
    delete[] m_vertices;
}

Crossing** CityGraph::crossings() {return m_vertices;}

//O(V)
bool CityGraph::hasSegment(vertex v1, vertex v2){
    EdgeNode* node = m_edges(v1);
    while(node){
        if(node->endVertex == v2){
            return true;
        }
        node = node->next;
    }
    return false;
}

//O(V) because in worst case it adds on the end of the list
void CityGraph::addSegment(vertex v1, vertex v2, EdgeNode* informations = nullptr){
    if(v1 >= 0 && v1 < m_numVertices && v2 >= 0 && v2 < m_numVertices){
        EdgeNode* node = m_edges(v1); //Get the list head of edges for that vertex
        if(node == nullptr || node->endVertex >= v2){
            m_vertices[v1]->segments = informations? informations : new EdgeNode;
            m_vertices[v1]->segments->next = node;
            m_vertices[v1]->segments->endVertex = v2;
            m_numEdges++;
        } else{
            while(node->next != nullptr && node->next->endVertex < v2) {
                node = node->next;
            }
            if(node->next == nullptr && node->endVertex != v2){
                node->next = informations? informations : new EdgeNode;
                node->next->next = nullptr;
                node->next->endVertex = v2;
                m_numEdges++;
            }
            else if (node->next->endVertex != v2){ //Avoid adding the same vertex two times
                EdgeNode* newVertex = informations? informations : new EdgeNode;
                newVertex->endVertex = v2;
                newVertex->next = node->next;
                node->next = newVertex;
                m_numEdges++;
            }
        }
    }
}

//O(V) because in worst case it goes trought every edge in a vertex
void CityGraph::removeSegment(vertex v1, vertex v2){
    if(v1 >= 0 && v1 < m_numVertices && v2 >= 0 && v2 < m_numVertices){
        EdgeNode* node = m_edges(v1);
        if(node == nullptr) return;
        else if(node->endVertex == v2){
            m_vertices[v1]->segments = node->next;
            delete node;
            m_numEdges--;
        }
        else{
            while(node->next != nullptr){
                if(node->next->endVertex == v2){
                    EdgeNode* old = node->next;
                    node->next = old->next;
                    delete old;
                    m_numEdges--;
                    break;
                }
                node = node->next;
            }
        }
    }
}

//O(V + E)
void CityGraph::print(){
    cout << m_numEdges << " [ ";
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* node = m_edges(i); //Get the list head of edges for that vertex
        while (node != nullptr) { //And goes printing everything
            cout << "(" << i << " " << node->endVertex << ") ";
            node = node->next;
        }
    }
    cout << "]" << endl;
}

//O(E + E') bacause it passes through each edge of the two graphs
bool CityGraph::isSubGraph(CityGraph& H) {
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* hEdge = H.m_edges(i);
        EdgeNode* gEdge = m_edges(i);
        while (hEdge) {
            while (gEdge != nullptr && gEdge->endVertex < hEdge->endVertex){
                gEdge = gEdge->next;
            }
            if(gEdge == nullptr){
                return false;
            }
            else if (gEdge->endVertex != hEdge->endVertex){
                return false;
            }
            hEdge = hEdge->next;
        }
    }
    return true;
}

int CityGraph::numRegions() {
    return m_numRegions;
}

int CityGraph::numNodes() {
    return m_numVertices;
}

//O(n * V) because it checks, for each step in the path, if an edge exists. Checking the existence of an edge can take len(V)
bool CityGraph::isValidPath(vertex path[], int iLength, bool& hasCycle){
    if (iLength < 2) return false;
    bool visited[m_numVertices];
    for(vertex i = 0; i <m_numVertices; i++) {visited[i] = false;}
    visited[path[0]] = true;

    for(int i = 1; i < iLength; i++){
        if (visited[path[i]]) hasCycle = true;

        EdgeNode* node = m_edges(path[i-1]);
        bool exists = false;
        while(node){
            if(node->endVertex == path[i]){
                exists = true;
                break;
            }
            node = node->next;
        }
        if(!exists){
            return false;
        }
        visited[path[i]] = true;
    }
    return true;
}

// //namespace F1 {
// int main(){

//     vertex path1[] = {0, 2, 4, 1};
//     vertex path2[] = {0, 2, 4, 1, 5};
//     vertex path3[] = {0, 2, 4, 1, 4, 1};

//     bool p1cycle = false;
//     bool p2cycle = false;
//     bool p3cycle = false;

//     CityGraph g(1000);

//     g.addSegment(0, 1);
//     g.addSegment(0, 2);
//     g.addSegment(1, 3);
//     g.addSegment(2, 4);
//     g.addSegment(3, 5);
//     g.addSegment(4, 5);
//     g.addSegment(4, 1);
//     g.print();

//     cout << endl;
//     cout << "path1 é caminho? " << g.isValidPath(path1, 4, p1cycle) << " Com ciclo? " << p1cycle << endl;
//     cout << "path2 é caminho? " << g.isValidPath(path2, 5, p2cycle) << " Com ciclo? " << p2cycle << endl;
//     cout << "path3 é caminho? " << g.isValidPath(path3, 6, p3cycle) << " Com ciclo? " << p3cycle << endl;

//     g.removeSegment(0, 1);
//     g.removeSegment(0, 2);
//     g.removeSegment(1, 3);
//     g.removeSegment(2, 4);
//     g.removeSegment(3, 5);
//     g.removeSegment(4, 5);
//     g.removeSegment(4, 1);
//     g.print();


//     //http://graphonline.top/en/?graph=EBRQQOMYwjhUngCi
//     CityGraph g7 = CityGraph(8);
//     EdgeNode* node = new EdgeNode;
//     node->lenght = 7;
//     g7.addSegment(7, 1, node);
//     EdgeNode* node1 = new EdgeNode;
//     node1->lenght = 5;
//     g7.addSegment(1, 2, node1);
//     EdgeNode* node2 = new EdgeNode;
//     node2->lenght = 14;
//     g7.addSegment(1, 5, node2);
//     EdgeNode* node3 = new EdgeNode;
//     node3->lenght = 1;
//     g7.addSegment(0, 4, node3);
//     EdgeNode* node4 = new EdgeNode;
//     node4->lenght = 6;
//     g7.addSegment(1, 4, node4);
//     EdgeNode* node5 = new EdgeNode;
//     node5->lenght = 2;
//     g7.addSegment(4, 1, node5);
//     EdgeNode* node6 = new EdgeNode;
//     node6->lenght = 11;
//     g7.addSegment(2, 7, node6);
//     EdgeNode* node7 = new EdgeNode;
//     node7->lenght = 1;
//     g7.addSegment(6, 5, node7);
//     EdgeNode* node8 = new EdgeNode;
//     node8->lenght = 6;
//     g7.addSegment(5, 6, node8);
//     EdgeNode* node9 = new EdgeNode;
//     node9->lenght = 14;
//     g7.addSegment(6, 0, node9);
//     EdgeNode* node10 = new EdgeNode;
//     node10->lenght = 14;
//     g7.addSegment(3, 1, node10);
//     EdgeNode* node11 = new EdgeNode;
//     node11->lenght = 2;
//     g7.addSegment(5, 1, node11);

//     vertex g7Parents[8];
//     int g7Distances[8];
//     g7.CPTDijkstra(0, g7Parents, g7Distances);
//     cout << "Menores distâncias partindo de 0 em g7: ";
//     printList(g7Distances, 8);
//     cout << "CPT: ";
//     printList(g7Parents, 8);

//     return 0;
// }//}
