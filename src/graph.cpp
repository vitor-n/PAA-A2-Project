#include <iostream>
#include "graph.h"
#define printList(v, n) {cout << "[ "; for (int i = 0; i < n; i++) { cout << v[i] << " "; }; cout << " ]" << endl;}

using namespace std;

Graph::Graph(int numVertices):
m_numVertices(numVertices), m_numEdges(0), m_edges(nullptr){
    m_edges = new EdgeNode*[numVertices]; //Initializes the list of edges for each vetex
    for (vertex i = 0; i < numVertices; i++) {
        m_edges[i] = nullptr; //No edges exist yet
    }
}

Graph::~Graph(){
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* node = m_edges[i]; //Get the list head of edges for that vertex
        while (node != nullptr) { //And goes deleting everything
            EdgeNode* nextNode = node->next;
            delete node;
            node = nextNode;
        }
    }
    delete[] m_edges;
}

EdgeNode** Graph::edges() {return m_edges;}

//O(V)
bool Graph::hasEdge(vertex v1, vertex v2){
    EdgeNode* node = m_edges[v1];
    while(node){
        if(node->vert == v2){
            return true;
        }
        node = node->next;
    }
    return false;
}

//O(V) because in worst case it adds on the end of the list
void Graph::addEdge(vertex v1, vertex v2){
    if(v1 >= 0 && v1 < m_numVertices && v2 >= 0 && v2 < m_numVertices){
        EdgeNode* node = m_edges[v1]; //Get the list head of edges for that vertex
        if(node == nullptr || node->vert >= v2){
            m_edges[v1] = new EdgeNode;
            m_edges[v1]->next = node;
            m_edges[v1]->vert = v2;
            m_edges[v1]->maxSpeed = 60;
            m_numEdges++;
        } else{
            while(node->next != nullptr && node->next->vert < v2) {
                node = node->next;
            }
            if(node->next == nullptr && node->vert != v2){
                node->next = new EdgeNode;
                node->next->next = nullptr;
                node->next->vert = v2;
                node->maxSpeed = 60;
                m_numEdges++;
            }
            else if (node->next->vert != v2){ //Avoid adding the same vertex two times
                EdgeNode* newVertex = new EdgeNode;
                newVertex->vert = v2;
                newVertex->next = node->next;
                node->next = newVertex;
                node->maxSpeed = 60;
                m_numEdges++;
            }
        }
    }
}

//O(V) because in worst case it goes trought every edge in a vertex
void Graph::removeEdge(vertex v1, vertex v2){
    if(v1 >= 0 && v1 < m_numVertices && v2 >= 0 && v2 <m_numVertices){
        EdgeNode* node = m_edges[v1];
        if(node == nullptr) return;
        else if(node->vert == v2){
            m_edges[v1] = node->next;
            delete node;
            m_numEdges--;
        }
        else{
            while(node->next != nullptr){
                if(node->next->vert == v2){
                    EdgeNode* old = node->next;
                    node->next = old->next;
                    delete old;
                    m_numEdges--;
                    break;
                }
            }
        }
    }
}

//O(V + E)
void Graph::print(){
    cout << m_numEdges << " [ ";
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* node = m_edges[i]; //Get the list head of edges for that vertex
        while (node != nullptr) { //And goes printing everything
            cout << "(" << i << " " << node->vert << " " << node->maxSpeed << ") ";
            node = node->next;
        }
    }
    cout << "]" << endl;
}

//O(E + E') bacause it passes through each edge of the two graphs
bool Graph::isSubGraph(Graph& H) {
    EdgeNode** hEdges = H.edges();
    for (vertex i = 0; i < m_numVertices; i++){
        EdgeNode* hEdge = hEdges[i];
        EdgeNode* gEdge = m_edges[i];
        while (hEdge) {
            while (gEdge != nullptr && gEdge->vert < hEdge->vert){
                gEdge = gEdge->next;
            }
            if(gEdge == nullptr){
                return false;
            }
            else if (gEdge->vert != hEdge->vert){
                return false;
            }
            hEdge = hEdge->next;
        }
    }
    return true;
}

//O(n * V) because it checks, for each step in the path, if an edge exists. Checking the existence of an edge can take len(V)
bool Graph::isValidPath(vertex path[], int iLength, bool& hasCycle){
    if (iLength < 2) return false;
    bool visited[m_numVertices];
    for(vertex i = 0; i <m_numVertices; i++) {visited[i] = false;}
    visited[path[0]] = true;

    for(int i = 1; i < iLength; i++){
        if (visited[path[i]]) hasCycle = true;

        EdgeNode* node = m_edges[path[i-1]];
        bool exists = false;
        while(node){
            if(node->vert == path[i]){
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

int main(){

    Edge ed = {0, 1, 120, 60, 123, 123};

    Graph g(1000);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);
    g.print();
}