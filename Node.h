/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
    private:
        Edge* first_edge;
        Edge* last_edge;
        int id;
        int label;
        unsigned int in_degree;
        unsigned int out_degree;
        float weight;
        Node* next_node;

    public:
        // Constructor
        Node(int id, int label);
        // Destructor
        ~Node();
        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getLabel();
        int getInDegree();
        int getOutDegree();
        float getWeight();
        Node* getNextNode();
        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);
        void setId(int id);
        void setLabel(int label);
        // Other methods
        bool searchEdge(int target_id);
        void insertEdge(int target_id, float weight);
        void removeAllEdges();
        int removeEdge(int id, bool directed, Node* target_node);
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        Edge* hasEdgeBetween(int target_id);
        // Auxiliar methods
        int arestaRepete(int id, float weight);

};

#endif // NODE_H_INCLUDED
