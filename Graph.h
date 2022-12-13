/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include "Edge.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

    public:
        //Constructor
        Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
        //Destructor
        ~Graph();
        //Getters
        int getOrder();
        int getNumberEdges();
        bool getDirected();
        bool getWeightedEdge();
        bool getWeightedNode();
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void imprimeGrafo();
        void insertNode(int id, float weight);
        void insertEdge(int id, int target_id, float weight);
        void insertEdgeAndNodeWeight(int id, int target_id, float edgeWeight, float nodeSourceWeight, float nodeTargetWeight);
        void removeNode(int id);
        bool searchNode(int id);
        Node* getNode(int id);
        Node* getNodeForced(int id, float weight);
        bool depthFirstSearch(int initialId,int targetId);
        void breadthFirstSearch(ofstream& output_file);
        Graph* getComplement();
        Graph* getSubjacent();
        bool hasCircuit();
        bool connectedGraph();
        float** floydMarshall();
        float* dijkstra(int id);
        Graph* graphIntersection(Graph* graph2);
        Graph* graphUnion(Graph* graph2);
        //auxiliar
        bool auxDepthFirstSearch(int initialId, int targetId, Node* p, Edge* e);

    private:
        //Auxiliar methods
        void inicializaGrafo();

};

#endif // GRAPH_H_INCLUDED
