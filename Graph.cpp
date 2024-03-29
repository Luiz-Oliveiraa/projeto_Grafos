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
        Node *primeiro = new Node(-1, 0);
        this->first_node = primeiro;
        this->last_node = primeiro;
        //cria nó com id = -1, no final da lista
        for(int i=1; i < this->order; i++){
            Node *auxNode = new Node(-1, i);
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
                Node *novo = new Node(id, this->order +1);
                novo->setWeight(weight);
                this->last_node->setNextNode(novo);
                this->last_node = novo;
                this->order += 1;
                break;
            }
        }
    }
    else{//caso não exista vertice no grafo, cria-se o primeiro
        Node *novo = new Node(id, this->order +1);
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
    this->number_edges +=1;

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
    if(this->first_node != nullptr){
        for(Node *p = this->first_node; p !=nullptr; p = p->getNextNode()){
            //se é para remover o primeiro nó
            if(p->getId() == id && p == this->first_node){
                if(!this->getDirected()){
                   for(Edge* edge = p->getFirstEdge();edge != nullptr; edge = edge->getNextEdge()){
                        Node* aux_node = this->getNode(edge->getTargetId());
                        aux_node->removeEdge(p->getId( ), this->getDirected(), p);
                    } 
                }
                this->first_node = p->getNextNode();
                p->removeAllEdges();
                delete p;
                this->order--;
                break;
            }

            //se é o próximo nó da lista que tem que ser removido
            if(p != this->last_node){
                if(p->getNextNode()->getId() == id){
                    Node *aux_node = p;
                    p = p->getNextNode();
                    aux_node->setNextNode(p->getNextNode());
                    //se é para remover o ultimo nó, atualiza o ultimo nó
                    if(p == this->last_node){
                        this->last_node = aux_node;
                    }

                    if(!this->getDirected()){
                        for(Edge* edge = p->getFirstEdge();edge != nullptr; edge = edge->getNextEdge()){
                            Node* aux_node = this->getNode(edge->getTargetId());
                            aux_node->removeEdge(p->getId( ), this->getDirected(), p);
                        } 
                    }
                    p->removeAllEdges();
                    delete p;
                    this->order--;
                    break;
                }
            }
            
        }
    }
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
                Node *aux = new Node(id, this->order +1);
                aux->setWeight(weight);
                p->setNextNode(aux);
                this->last_node = aux;
                this->order += 1;
                return aux;
            }
        } 
    }
    //se o grafo ainda não possuí nos
    //else{
        Node *novo = new Node(id, this->order +1);
        novo->setWeight(weight);
        this->first_node = novo;
        this->last_node = novo;
        this->order += 1;
        return novo;
    //}
    
}

bool Graph::ordenacaoTopologica(){

    int visitado[order] = {0}; //label para indicar se um nó foi visitado
    bool ciclico = false;
    stack<int> pilha;

    Node* aux = first_node;
    for(int i=0; aux != nullptr && i<order; i++, aux = aux->getNextNode()){
        if(visitado[i] == 0)
            auxOrdenacaoTopologica(visitado, i, aux, ciclico, pilha); //Busca em profoundidade adaptada, armazena valores numa pilha
    }   

    Node* no_a = first_node;
    Node* anteriorA = nullptr;

    Node* no_b = first_node->getNextNode();
    Node* anteriorB = first_node;

   
    //Trocar as posições dos nós, para ordenar o grafo
    for( ;no_a != nullptr && pilha.size() > 0; no_a = no_a->getNextNode()){
        if(no_a->getId() != pilha.top()){
            for(no_b = no_a->getNextNode(), anteriorB = no_a; no_b != nullptr; no_b = no_b->getNextNode(), anteriorB = anteriorB->getNextNode()){
                if(no_b->getId() == pilha.top()){

                    Node* proxA = no_a->getNextNode();
                    Node* proxB = no_b->getNextNode();

                    no_a->setNextNode(proxB);

                    if(no_a == first_node){
                        first_node = no_b;
                    }else{
                        anteriorA->setNextNode(no_b);
                    }

                    if(proxB == nullptr){
                        last_node = no_a;
                    }

                    if(proxA == no_b){
                        no_b->setNextNode(no_a);
                    }else{
                        no_b->setNextNode(proxA);
                        anteriorB->setNextNode(no_a);
                    }
                    no_a = no_b; //resetando posição do ponteira na lista encadeada
                    break;
                }
                
            }
        }
        pilha.pop(); 
        anteriorA = no_a;
       
    }

    return ciclico;
}

//busca em profundidade adaptada, armazena valores numa pilha
void Graph::auxOrdenacaoTopologica(int visitado[], int i, Node* no, bool &ciclico, stack<int> &pilha){
    visitado[i] = 1;

    for(Edge* e = no->getFirstEdge(); e!= nullptr; e = e->getNextEdge()){
        int j = 0;
        this->getNode(5);
        for(Node* n = first_node; n!= nullptr && j < order; n = n->getNextNode(), j++){
            if(n->getId() == e->getTargetId()){
                if(visitado[j] == 0){
                    auxOrdenacaoTopologica(visitado, j, n, ciclico, pilha);
                }else{
                    if(visitado[j] == 1){
                        ciclico = true;
                    }
                    
                }
            }
        }
    }

    pilha.push(no->getId());
    visitado[i] = 2;
    
}

bool Graph::verificaPossibilidadeDePert(ofstream& output_file){
    if(!output_file.is_open()){
        cout << "Não foi possivel abrir o arquivo de saida" << endl;
        return 0;
    }
    output_file << endl << "/*" << endl;
    output_file << endl << "____________REDE PERT____________" << endl << endl;

    if(!getDirected()){
        output_file << endl << "Não foi possivel realizar o pert, pois o grafo não é direcionado" << endl;
        output_file << endl << "*/" << endl;
        return 0;
    }
    if(!getWeightedEdge()){
        output_file << endl << "Não foi possivel realizar o pert, pois o grafo não tem arestas ponderadas" << endl;
        output_file << endl << "*/" << endl;
        return 0;
    }

    int inicio = 0;
    int fim = 0;
    //conta se o grafo tem mais de um "nó inicio" ou mais de um "nó final"
    for(Node* p = first_node; p!=nullptr; p = p->getNextNode()){
        if(p->getInDegree() <= 0){
            fim++;
            if(fim > 1){
                output_file << endl << "Não foi possivel realizar o pert, pois o grafo tem mais de uma 'etapa saída'" << endl;
                output_file << endl << "(mais de um nó que não inicia tarefas)" << endl;
                output_file << endl << "*/" << endl;
                return 0;
                break;
            }
        }
        if(p->getOutDegree() <= 0){
            inicio++;
            if(inicio > 1){
                output_file << endl << "Não foi possivel realizar o pert, pois o grafo tem mais de uma 'etapa inicio'" << endl;
                output_file << endl << "(mais de um nó que não recebe tarefas)" << endl;
                output_file << endl << "*/" << endl;
                return 0;
                break;
            }
        }
    }

    if(ordenacaoTopologica()){
        output_file << endl << "Não foi possivel realizar o pert, pois o grafo é ciclico" << endl;
        output_file << endl << "*/" << endl;
        return 0;
    }

    return true;
}


bool Graph::pert(ofstream& output_file){
    if(!verificaPossibilidadeDePert(output_file))
        return 0;

    int menorTempo[order] = {};
    int maiorTempo[order];
    int vetor[order];
    stack<int> pilha; //auxiliar usado para criar um grafo com as arestas invertidas

    ///calculando tempo mínimo
    Node* p;
    int i=0;
    for(p = first_node, i=0; p!= nullptr; p = p->getNextNode(), i++){
        vetor[i] = p->getId();
        pilha.push(p->getId());
        for(Edge* e = p->getFirstEdge(); e!=nullptr; e = e->getNextEdge()){
            int indice = 0;
            for(Node* q = first_node; q!= nullptr; q = q->getNextNode(), indice ++){
                if(e->getTargetId() == q->getId()){
                    if(menorTempo[indice] < e->getWeight()+ menorTempo[i]){
                        menorTempo[indice] = e->getWeight() + menorTempo[i];
                        break;
                    }
                }
            }
        }
    }

    //criando um grafo auxiliar com arestas invertiads
    Graph graphAux(order, getDirected(), getWeightedEdge(), getWeightedNode());
    for(;pilha.size() > 0; pilha.pop()){
        graphAux.insertNode(pilha.top(), 0);
    }

    for(Node* p = first_node; p!= nullptr; p = p->getNextNode()){
        for(Edge* e = p->getFirstEdge(); e!=nullptr; e = e->getNextEdge()){
            graphAux.insertEdge(e->getTargetId(), p->getId(), e->getWeight());
        }
    }

    ///calculando tempo máximo
    for(int j = 0; j<order; j++)
        maiorTempo[j] = {menorTempo[order-1]};

    for(p = graphAux.first_node, i= order-1; p!= nullptr; p = p->getNextNode(), i--){
        for(Edge* e = p->getFirstEdge(); e!=nullptr; e = e->getNextEdge()){
            int indice = order-1;
            for(Node* q = graphAux.first_node; q!= nullptr; q = q->getNextNode(), indice --){
                if(e->getTargetId() == q->getId()){
                    if(maiorTempo[indice] > maiorTempo[i] - e->getWeight()){
                        maiorTempo[indice] = maiorTempo[i] - e->getWeight();
                        break;
                    }
                }
            }
        }
    }
    ///ordenando menor caminho
    int menor, aux;
    for(i=0;i<order;i++){
        menor = i;
        for(int j=i+1; j<order; j++){
            if(menorTempo[menor]>menorTempo[j])
                menor = j;
        }
        if(i != menor){
            aux = menorTempo[i];
            menorTempo[i] = menorTempo[menor];
            menorTempo[menor] = aux;

            aux = maiorTempo[i];
            maiorTempo[i] = maiorTempo[menor];
            maiorTempo[menor] = aux;

            aux = vetor[i];
            vetor[i] = vetor[menor];
            vetor[menor] = aux;
        }
    }
    ///imprimindo na saida
    output_file << "Ordem de execução:" << endl << endl;
    for(i=0; i<order; i++){
        output_file << "tarefa "<< vetor[i] << ":\ttempo mínimo de ínicio: "<< menorTempo[i];
        output_file << "\ttempo máximo de ínicio: " << maiorTempo[i] << endl;    
    }
    if(order>0){
        output_file << endl <<"Tempo Total de execução: " << menorTempo[order-1] << endl;
        
        output_file << "_________________________________" << endl;
    }

    output_file << endl << "*/" << endl;
    return 1;
}


//Function that verifies if there is a path between two nodes
bool Graph::depthFirstSearch(int initialId, int targetId){
    Node* p = NULL;
    return auxDepthFirstSearch(initialId, targetId, p);
}

bool Graph::auxDepthFirstSearch(int initialId, int targetId, Node* p){
    // encontra o nó inicial e depois não volta mais para essa verificão, se não voltaria sempre para o initialID
    if(p == NULL){
        for(Node* q = this->first_node; q != nullptr; q = q->getNextNode()){
            if(q->getId() == initialId)
                p = q;
        }
    }
    if(p->getId() != targetId)
        return auxDepthFirstSearch(initialId, targetId, p->getNextNode());
    return true;
}


// Alterações no Grafo Interseção
Graph* Graph::graphIntersection(Graph* graph2){
    Graph* novo_grafo = new Graph(0, false, false, false);
    if(this->order > 0 && graph2->order > 0 && graph2->number_edges > 0 && this->number_edges > 0 && graph2->directed == directed){
        Node* p = this->first_node; //ponteiro pra percorrer os nós do grafo 1
        Node* k = graph2->first_node; //ponteiro pra percorrer os nós do grafo 2
        Edge* e = p->getFirstEdge(); //ponteiro pra percorrer as arestas do grafo 1
        Edge* f = k->getFirstEdge();//ponteiro pra percorrer as arestas do grafo 2
        // inicialização do novo_grafo
        novo_grafo->order = 0;
        novo_grafo->number_edges = 0;
        for(p = this->first_node; p != nullptr; p = p->getNextNode()){ //passa pelos nós do primeiro grafo
            for(k = graph2->first_node; k != nullptr; k = k->getNextNode()){//passa pelos nós do segundo grafo
                if(p->getId() == k->getId()){
                    novo_grafo->insertNode(p->getId(), p->getWeight());//adiciona os nós que forem iguais ao novo grafo
                    for(e = p->getFirstEdge(); e != nullptr; e = e->getNextEdge()){//passa pelas arestas do primeiro grafo
                        for (f = k->getFirstEdge(); f != nullptr; f = f->getNextEdge()){//passa pelas arestas do segundo grafo
                            if(e->getTargetId() == f->getTargetId()){//compara se as arestas dão no mesmo nó para adicionar
                                if(p->getId() != -1 || p->getFirstEdge() != nullptr){
                                    if(!verificaAresta(novo_grafo, p, e->getTargetId())){
                                        novo_grafo->insertEdge(p->getId(), e->getTargetId(), e->getWeight()); 
                                    }                                    
                                }
                                
                            }
                        }
                    }
                }
            }
        }
        cout << "Número de arestas do novo grafo: " << novo_grafo->number_edges << endl;
        return novo_grafo;
    }
    else{
        cout << "ERRO: Grafo inválido." << endl;
        return novo_grafo;
    }
}

 Graph* Graph::graphDiference(Graph* graph2)
 {
    if (this->order == graph2->order && graph2->number_edges <= this->number_edges) //checando se o grafo 2 é valido
    {
        Graph* novo_grafo = new Graph(0, false, false, false);
        Node* p = this->first_node; // ponteiro que percorrerá os vértices do grafo 1
        Node* k = graph2->first_node; // ponteiro que percorrerá os vértices do grafo 2
        Edge* e = p->getFirstEdge(); // ponteiro que percorrerá as arestas do grafo 1
        Edge* f = k->getFirstEdge(); // ponteiro que percorrerá as arestas do grafo 2
        novo_grafo->order = 0;
        novo_grafo->number_edges = 0;
        for(p = this->first_node; p != nullptr; p = p->getNextNode()){ //inserindo todos os vértices do grafo 1 no novo grafo
            novo_grafo->insertNode(p->getId(), p->getWeight());
            for (k = graph2->first_node; k != nullptr; k = k->getNextNode()) { 
                if (p->getId() == k->getId()) { 
                    for(e = p->getFirstEdge();e != nullptr; e = e->getNextEdge()){ //inserindo no novo grafo as arestas do grafo 1 que não tem no grafo 2 
                        bool diferente = true;
                        for(f = k->getFirstEdge();f != nullptr; f = f->getNextEdge()){
                            if(e->getTargetId() == f->getTargetId()) {
                                diferente = false;
                                break;
                            }
                            else diferente = true;
                        }
                        Node* aux = novo_grafo->getNode(p->getId());
                        if (diferente) { //se for diferente  inserir a aresta
                            aux->insertEdge(e->getTargetId(), e->getWeight());
                            novo_grafo->number_edges++;
                        }
                    }
                }
            }
        }
        return novo_grafo;
    }
    else {
        cout << "ERRO: Grafo inválido. Numero de vértices diferente entre os grafos ou numero de arestas do grafo 2 maior do que o do grafo 1" << endl;
        exit(1);
    }
 }
bool Graph::verificaAresta(Graph* g, Node *p, int targetid) // função auxiliar para verificar se já existe a aresta no grafo
{
    Node *aux;
    for(aux = g->first_node; aux != nullptr; aux = aux->getNextNode())
    {
        if(aux->getId() == p->getId())
            break;
    } 
    return aux->searchEdge(targetid);
}

Graph* Graph::graphUnion(Graph* graph2)
{
    Graph* graph_union = new Graph(0, false, false, false);
    Node* n = this->first_node; // aponta para os vértices do primeiro grafo
    Edge* e = n->getFirstEdge(); // aponta para as arestas do primeiro grafo
    Node* p = graph2->first_node; // aponta para os vértices do segundo grafo
    Edge* k = p->getFirstEdge(); // aponta para as arestas do segundo grafo
    graph_union->order = 0;
    graph_union->number_edges = 0;
    for(n = this->first_node; n != nullptr; n = n->getNextNode()) 
    {
        graph_union->insertNode(n->getId(), n->getWeight()); // inserindo todos os vértices do primeiro grafo
        
        for(e = n->getFirstEdge(); e != nullptr; e = e->getNextEdge()) 
        {           
            Node* aux = graph_union->getNode(n->getId());
            if(n->getId() != -1 || n->getFirstEdge() != nullptr)
            {
                    aux->insertEdge(e->getTargetId(), e->getWeight()); // inserindo todas as arestas do primeiro grafo
                    graph_union->number_edges++;                
            }                                             
        }      
    }   
    for(p = graph2->first_node ; p != nullptr; p = p->getNextNode()) 
    {
        graph_union->insertNode(p->getId(), p->getWeight()); // inserindo todos os vértices do segundo grafo que não estão no primeiro grafo
        
        for(k = p->getFirstEdge(); k != nullptr; k = k->getNextEdge())
        {
            Node* aux = graph_union->getNode(p->getId());
            if(p->getId() != -1 || p->getFirstEdge() != nullptr)
            {
                if(!verificaAresta(graph_union, p, k->getTargetId())) // verificando se a aresta do segundo grafo já contém no grafo união
                {
                    aux->insertEdge(k->getTargetId(), k->getWeight()); // inserindo todos as arestas do segundo grafo que não estão no primeiro grafo.
                    graph_union->number_edges++;  
                }                                    
            }
        }        
    }
    cout << "total de edges: " << graph_union->number_edges << endl;

    return graph_union;
}
