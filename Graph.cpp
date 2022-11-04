#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    this->inicializaGrafo();
}

// Destructor
Graph::~Graph()
{
    //cout << endl << "teste destrutor graph" << endl;

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}
//Auxiliar private methods

//alocanco espaço na memoria
void Graph::inicializaGrafo(){
    if(this->order > 0){
        //cria nó com id = -1, para indicar que o id não foi inicializado
        Node *primeiro = new Node(-1);
        this->first_node = primeiro;
        this->last_node = primeiro;
        //cria nó com id = -1, no final da lista
        for(int i=1; i < this->order; i++){
            Node *auxNode = new Node(-1);
            this->last_node->setNextNode(auxNode);
            this->last_node = auxNode;
        }  
    }
    
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}

bool Graph::getDirected()
{

    return this->directed;
}

bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

bool Graph::getWeightedNode()
{

    return this->weighted_node;
}

Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::imprimeGrafo(){
    cout << endl << "Grafo: " << endl;
    cout << "Ordem: " << this->order << ", Numero de arestas: " << this->number_edges << endl;
    if(this->first_node != nullptr){
        for(Node *aux_node = this->first_node; aux_node != nullptr; aux_node = aux_node->getNextNode()){
            cout << aux_node->getId() << " -> ";
            for(Edge *e = aux_node->getFirstEdge(); e != nullptr; e = e->getNextEdge()){
                cout << "|id: " << e->getTargetId() << "|p: " << e->getWeight() <<"| - ";
            }
            cout << "> nullptr" << endl;
        }
    }
}

void Graph::insertNode(int id, float weight)
{
    //se o grafo já possuí vértices
    if(this->first_node != nullptr){
        for(Node *aux = this->first_node; aux !=nullptr; aux = aux->getNextNode()){
            //se o nó foi inicializado, porem não tem id, adiciono o id
            if(aux->getId() == -1){
                aux->setId(id);
                aux->setWeight(weight);
                break;
            }
            //se o nó já está no grafo, só insiro o peso
            if(aux->getId() == id){
                aux->setWeight(weight);
                break;
            }
            //se o nó não está no grafo, insiro no final da lista
            if(aux == this->last_node){
                Node *novo = new Node(id);
                novo->setWeight(weight);
                this->last_node->setNextNode(novo);
                this->last_node = novo;
                this->order += 1;
                break;
            }
        }
    }
    else{//caso não exista vertice no grafo, cria-se o primeiro
        Node *novo = new Node(id);
        novo->setWeight(weight);
        this->first_node = novo;
        this->last_node = novo;
        this->order += 1;
    }
}

void Graph::insertEdge(int id, int target_id, float weight){
    Node*p = this->getNodeForced(id, 0); //cria nó1
    p->insertEdge(target_id, weight);
    p->incrementOutDegree();
    this->number_edges +=1;

    Node *p2 = this->getNodeForced(target_id, 0); //cria nó2
    p2->incrementInDegree();

    // se não for uma lista direcionada, adiciona a aresta na lista do nó2
    if(!this->getDirected()){
        p2->insertEdge(id, weight);
        p2->incrementOutDegree();
        p->incrementInDegree();
    }
}

void Graph::insertEdgeAndNodeWeight(int id, int target_id, float edgeWeight, float nodeSourceWeight, float nodeTargetWeight){
    Node*p = this->getNodeForced(id, nodeSourceWeight); //cria nó1
    p->insertEdge(target_id, edgeWeight);
    p->incrementOutDegree();

    Node *p2 = this->getNodeForced(target_id, nodeTargetWeight); //cria nó2
    p2->incrementInDegree();

    // se não for uma lista direcionada, adiciona a aresta na lista do nó2
    if(!this->getDirected()){
        p2->insertEdge(id, edgeWeight);
        p2->incrementOutDegree();
        p->incrementInDegree();
    }
}

void Graph::removeNode(int id){ 
    //corrigir bug
    /*for(Node *p = this->first_node; p !=nullptr; p = p->getNextNode()){
        //se é para remover o primeiro nó
        if(p->getId() == id && p == this->first_node){
            this->first_node = this->first_node->getNextNode();
            p->removeAllEdges();
            delete p;
            break;
        }
        //se é o próximo nó da lista que tem que ser removido
        if(p != this->last_node){
            if(p->getNextNode()->getId() == id){
                cout<<"teste1"<<endl;
                Node *aux_node = p;
                p = p->getNextNode();
                aux_node->setNextNode(p->getNextNode());
                //se é para remover o ultimo nó, atualiza o ultimo nó
                if(p == this->last_node){
                    this->last_node = aux_node;
                }
                cout<<"teste2"<<endl;
                p->removeAllEdges();
                cout<<"teste3"<<endl;
                delete p;
                cout<<"testeFinal"<<endl;
                break;
            }
        }
        
    }
    */
}

bool Graph::searchNode(int id)
{
    for(Node *p = this->first_node; p !=nullptr; p = p->getNextNode()){
        if(p->getId() == id)
            return true;
    }
    return false;
}

Node *Graph::getNode(int id)
{
    for(Node *p = this->first_node; p !=nullptr; p = p->getNextNode()){
        if(p->getId() == id)
            return p;
    }
    //obs, necessario fazer tratanento de erro
    return nullptr;
    
}

//procura o nó no grafo, se ele não existir criamos ele
Node *Graph::getNodeForced(int id, float weight)
{
    if(this->first_node != nullptr){
       for(Node *p = this->first_node; p !=nullptr; p = p->getNextNode()){
            //se o nó não está no grafo, porém já foi inicializado no construtor
            if(p->getId() == -1){
                p->setId(id);
                p->setWeight(weight);
                return p;
            }
            //se o nó está no grafo
            if(p->getId() == id)
                return p;
            //se o nó não está no grafo e nem foi inicializado no construtor
            if(this->last_node == p){
                Node *aux = new Node(id);
                aux->setWeight(weight);
                p->setNextNode(aux);
                this->last_node = aux;
                this->order += 1;
                return aux;
            }
        } 
    }
    //se o grafo ainda não possuí nos
    else{
        Node *novo = new Node(id);
        novo->setWeight(weight);
        this->first_node = novo;
        this->last_node = novo;
        this->order += 1;
        return novo;
    }
    
}


//Function that verifies if there is a path between two nodes
bool Graph::depthFirstSearch(int initialId, int targetId){
    
}


void Graph::breadthFirstSearch(ofstream &output_file){
    
}


Graph *Graph::getComplement(){
    
}

    

//A function that returns a subjacent of a directed graph, which is a graph which the arcs have opposite directions to the original graph
Graph* Graph::getSubjacent(){
    
}

bool Graph::connectedGraph(){
    
}



bool Graph::hasCircuit(){
    
}



float** Graph::floydMarshall(){
    
}

   

float* Graph::dijkstra(int id){
    
}
