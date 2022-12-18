#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

/*

Autores: 
Thiago Silva Murta
Luiz Oliveira
André Dutra
Matheus Peron

Comandos ara executar o programa no terminal:
Windows:
g++ Edge.cpp Node.cpp Graph.cpp main.cpp -o execGrupo6
.\execGrupo6.exe entrada.txt saida.txt 0 0 0
Linux:
g++ Edge.cpp Node.cpp Graph.cpp main.cpp -o execGrupo6
./execGrupo6 entrada.txt saida.dot 0 0 0

Para gerar imagem, a saida tem que ser .dot
depois de executar o programa, no terminal digite:
dot -Tpng -O saida.dot

Parâmetros:
executavel,
nome_da_entrada.txt,
nome_da_saida.dot,
booleano se é um grafo direcionado,
booleano se tem aresta ponderado,
booleano se tem vertice ponderado

vc tem que deixar uma entrada.txt na pasta onde sera executado o programa

Teste commit
*/

Graph* leitura(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo, lendo a primeira linha
    input_file >> order;

    //Criando objeto grafo
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    // aresta não ponderada e vertice não ponderado
    if(!graph->getWeightedEdge() && !graph->getWeightedNode()){

        while(input_file >> idNodeSource >> idNodeTarget) { // lendo uma linha da entrada.txt e pegando a aresta (2 vertices)
            //se existe o segundo nó
            if(idNodeTarget!= -1){
                graph->insertEdge(idNodeSource, idNodeTarget, 0); // criando aresta, como não é ponderada, enviamos 0
            }
            //se não existe o segundo nó
            else{
                graph->insertNode(idNodeSource, 0); //cria apenas o nó
            }
        }

    //aresta ponderada e vertice não ponderado
    }else if(graph->getWeightedEdge() && !graph->getWeightedNode() ){

        float edgeWeight;

        while(input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {
            if(idNodeTarget!= -1){
                graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);//cria dois nos e a aresta
            }
            //se não existe o segundo nó
            else{
                graph->insertNode(idNodeSource, 0); //cria apenas o nó
            }

        }

    //nó ponderado e aresta não ponderada:
    }else if(graph->getWeightedNode() && !graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
            if(idNodeTarget!= -1){
                graph->insertEdgeAndNodeWeight(idNodeSource, idNodeTarget, 0, nodeSourceWeight, nodeTargetWeight);//cria dois nos e a aresta
            }
            //se não existe o segundo nó
            else{
                graph->insertNode(idNodeSource, nodeSourceWeight); //cria apenas o nó
            }
        }

    //nó ponderado e aresta ponderada
    }else if(graph->getWeightedNode() && graph->getWeightedEdge()){

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while(input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight >> edgeWeight) {
            if(idNodeTarget!= -1){
                graph->insertEdgeAndNodeWeight(idNodeSource, idNodeTarget, edgeWeight, nodeSourceWeight, nodeTargetWeight);//cria dois nos e a aresta
            }
            //se não existe o segundo nó
            else{
                graph->insertNode(idNodeSource, nodeSourceWeight); //cria apenas o nó
            }
        }
    }

    return graph;
}


//pode escrever varios grafos na mesma saida; para gerar imagem, a saida tem que ser .dot
void saida(ofstream& output_file, Graph* graph){
    if(output_file.is_open()){
        if(!graph->getDirected())
            output_file << "graph grafo{" << endl;
        else
            output_file << "digraph grafo{" << endl;

        //-----Personalizar layout (circo, dot, fdp, neato, osage, patchwork, sfdp, twopi)
        //output_file << "\tlayout = circo;" << endl;
        //output_file << "\tnode [style = filled, color = yellow]" << endl; //personalizar nó
        //output_file << "\tedge [color = red]" << endl << endl; //personalizar aresta
        
        if(graph->getWeightedNode()){
            for(Node* p = graph->getFirstNode(); p!=nullptr; p = p->getNextNode()){
                output_file << "\t" << p->getLabel() << "[label=\"" << p->getId() << "| p:" << p->getWeight() << "\"]" << endl;
            }
        }else{
            for(Node* p = graph->getFirstNode(); p!=nullptr; p = p->getNextNode()){
                output_file << "\t" << p->getLabel() << "[label=\"" << p->getId() << "\"]" << endl;
            }
        }

        output_file << endl;
        if(!graph->getDirected()){
            //estrutura auxiliar para diferenciar arestas repetidas de multiarestas
            int num_multiarestas[graph->getNumberEdges()] = {0};
            int nodeSource[graph->getNumberEdges()];
            int nodeTarget[graph->getNumberEdges()];

            for(Node* p = graph->getFirstNode(); p!=nullptr; p = p->getNextNode()){
                for(Edge* edge = p->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){

                    int nodeA = p->getId(), nodeB = edge->getTargetId();
                    bool imprime = false;

                    for(int i=0;i<graph->getNumberEdges();i++, imprime = false){
                        if(num_multiarestas[i] == 0){
                            nodeSource[i] = nodeA;
                            nodeTarget[i] = nodeB;
                            num_multiarestas[i]++;
                            imprime = true;
                        }else{
                            if(nodeSource[i] == nodeA && nodeTarget[i] == nodeB){
                                num_multiarestas[i]++;
                                imprime = true;
                            }else if(nodeTarget[i] == nodeA && nodeSource[i] == nodeB){
                                imprime = false;
                                break;
                            }
                        }
                        if(imprime){
                            if(graph->getWeightedEdge()){
                                output_file << "\t" << p->getLabel() << " -- " << graph->getNode(edge->getTargetId())->getLabel();
                                output_file << "[label=\"" << edge->getWeight() << "\"]" << endl;
                            }else{
                                output_file << "\t" << p->getLabel() << " -- " << graph->getNode(edge->getTargetId())->getLabel() << endl;
                            }
                            break;
                        }
                    }
                }
            } 
        }else{
            for(Node* p = graph->getFirstNode(); p!=nullptr; p = p->getNextNode()){
                for(Edge* edge = p->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                    if(graph->getWeightedEdge()){
                        output_file << "\t" << p->getLabel() << " -> " << graph->getNode(edge->getTargetId())->getLabel();
                        output_file << "[label=\"" << edge->getWeight() << "\"]" << endl;
                    }else{
                        output_file << "\t" << p->getLabel() << " -> " << graph->getNode(edge->getTargetId())->getLabel() << endl;
                    }
                }
            }
        }
        
        output_file << endl << "}" << endl;
    }

     else
            cout << "Não foi possivel abrir o arquivo de saida" << endl;
}

Graph* criaGrafo(){
    cout << endl;
    int selecao[3] = {-1};
    
    system("clear"); //apaga tudo que esta no terminal
    cout << "Criando novo grafo..." << endl;
    for (int i = 0; i < 3; i++)
    {
        int opcao = -1;
        while(opcao != 0 && opcao != 1){
            if(i == 0)
                cout << "O grafo é ponderado?" << endl;
            if(i == 1)
                cout << "O grafo possuí arestas ponderados?" << endl;
            if(i == 2)
                cout << "O grafo possuí vértices ponderados?" << endl;
            
            cout << "[0] para NÃO" << endl << "[1] para SIM" << endl;
            cin >>  opcao;
            system("clear"); //apaga tudo que esta no terminal
            cout << "Criando novo grafo..." << endl;
        }
        selecao[i] = opcao;
    }

    cout << "Digite o nome do arquivo de entrada. (exemplo: entrada2.txt)" << endl;
    string name;
    cin.get();
    getline(cin, name);

    ifstream input_file;
    input_file.open(name, ios::in);

    Graph* graph;

    if(input_file.is_open()){
        graph = leitura(input_file, selecao[0], selecao[1], selecao[2]);
        input_file.close();
        return graph;
    }else{
        cout << "Unable to open " << name << endl;
        exit(1);
    }        
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Complementar do grafo" << endl;
    cout << "[2] Imprimir caminhamento em largura" << endl;
    cout << "[3] Busca em profundidade" << endl;
    cout << "[4] Imprimir componentes conexas" << endl;
    cout << "[5] Imprimir componentes fortemente conexas" << endl;
    cout << "[6] Imprimir ordenacao topológica" << endl;
    cout << "[7] Árvore Geradora Mínima de Prim" << endl;
    cout << "[8] Caminho Mínimo Dijkstra" << endl;
    cout << "[9] Caminho Mínimo Floyd" << endl;
    cout << "[10] Algoritmos Gulosos (Abre um submenu)" << endl;
    cout << "Funcionalidades da parte 1 do trabalho:" << endl;
    cout << "[11] Grafo interseção" << endl;
    cout << "[12] Grafo União" << endl;
    cout << "[13] Grafo Diferença" << endl;
    cout << "[14] Rede Pert" << endl;
    cout << "Funcionalidades Auxiliares" << endl;
    cout << "[15] Imprime Grafo" << endl;
    cout << "[16] Grafo Subjacente" << endl;
    cout << "[17] Existe caminho entre os 2 nós?" << endl;
    cout << "[18] teste da função cria novo grafo (opcao provisoria)" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Complementar
        case 1:{
            
            break;
        }

        //BFS
        case 2:{
           
            break;
        }

        //DFS
        case 3:{
            
            break;
        }

        //Componentes Conexas
        case 4:{

            break;
        }

        //Componentes Fortementes Conexas
        case 5:{
            
            break;
        }

        //Ordenação Topológica
        case 6:{
            
            break;
        }

        case 7:{

            break;
        }

        //Algoritmo de Prim
        case 8:
        {
            
            break;
        }

        //Algoritmo de Dijkstra
        case 9:
        {
            
            break;
        }

        //Algoritmo de Floyd
        case 10:
        {
            
            break;

        }

        //Grafo Interseção
        case 11:
        {
            cout << "--------Grafo Interseção--------" << endl;
            Graph* graph2 = criaGrafo();
            saida(output_file, graph2);
            Graph* graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphIntersection(graph2);
            graph->imprimeGrafo();
            saida(output_file, graph3);
            delete graph3;
            delete graph2;
            break;

        }

        //Grafo União
        case 12:
        {
            cout << "--------Grafo União--------" << endl;
            Graph *graph2 = criaGrafo();
            saida(output_file, graph2);
            Graph *graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphUnion(graph2);
            saida(output_file, graph3);
            delete graph3;
            delete graph2;
            break;   
        }

        //Grafo Diferença
        case 13:
        {
            cout << "--------Grafo Diferença--------" << endl;
            Graph* graph2 = criaGrafo();
            saida(output_file, graph2);
            Graph* graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphDiference(graph2);
            graph3->imprimeGrafo();
            saida(output_file, graph3);
            delete graph3;
            delete graph2;
            break;

        }

        //Rede Pert
        case 14:
        {
            
            break;

        }

        //imprime Grafo
        case 15:
        {
            cout << "primeiro noh: " << graph->getFirstNode()->getId() <<" | ultimo noh: " << graph->getLastNode()->getId();
            graph->imprimeGrafo();
            //system("pause");
            break;
        }
        //
        case 16:{
            /*
            Graph* novo = new Graph(0, false, false, false);
            cout << "-Grafo Subjacente-" << endl;
            novo->getSubjacent();
            novo->imprimeGrafo();
            */
            break;
        }
        case 17:{
            cout << "-Caminho entre 2 nós-" << endl;
            cout << "Gostaria de verificar se existe um caminho entre os nós: " << endl;
            int no1, no2;
            cin >> no1 >> no2;
            if(!graph->depthFirstSearch(no1, no2))
                cout << "NÃO EXISTE!(FALSE)" << endl;
            else
                cout << "EXISTE!(TRUE)" << endl;
            break;
        }
        case 18:{
            //exemplo de criacao de grafo:
            Graph* grafo2;
            grafo2 = criaGrafo();

            //se quiser adicionar na saida para gerar imagem:
            saida(output_file, grafo2);

            //não esquecer de desalocar o grafo
            delete grafo2;
        }
  }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        //system("clear");
        selecao = menu();

        if(output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

    }
    if(selecao == 0){
        saida(output_file, graph);
        system("dot -Tpng -O saida.dot");
        return 0;
    }
        
    return 0;
}



int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    

    Graph* graph;

    if(input_file.is_open()){

        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];

    
    mainMenu(output_file, graph);

    

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    delete graph;

    return 0;
}

