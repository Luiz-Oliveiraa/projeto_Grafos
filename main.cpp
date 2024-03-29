#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <list>
#include "Graph.h"
#include "Node.h"

using namespace std;

//auxiliares para geração das saidas
//nomes das saidas
list<string> listaSaidas;
list<string> saidasPert; 
//quantidade de saidas
int contUniao = 0;
int contDiferenca = 0;
int contInterseção = 0;
int contPert = 0;

/*

Autores: 
Thiago Silva Murta
Luiz Oliveira
André Dutra
Matheus Peron

Comandos ara executar o programa no terminal:
Linux:
g++ Edge.cpp Node.cpp Graph.cpp main.cpp -o execGrupo6
./execGrupo6 entrada.txt saida.dot 0 0 0

Parâmetros:
executavel,
nome_da_entrada.txt,
nome_da_saida.dot,
booleano se é um grafo direcionado,
booleano se tem aresta ponderado,
booleano se tem vertice ponderado

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
void saida(ofstream& output_file, Graph* graph, string nome){
    bool adicionar = true;
    for(auto it = listaSaidas.begin(); it!=listaSaidas.end(); it++){
        if(*it == nome){
            adicionar = false;
            break;
        }
    }

    if(adicionar){
        listaSaidas.push_back(nome);
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
}

Graph* criaGrafo(ofstream& output_file, bool direcionado, bool arestaPonderada, bool verticePonderado){
    cout << endl;
    int selecao[3] = {-1};
    
    system("clear"); //apaga tudo que esta no terminal
    cout << "Criando novo grafo..." << endl;

    cout << "Digite o nome do arquivo de entrada. (exemplo: entrada2.txt)" << endl;
    string name;
    cin.get();
    getline(cin, name);

    ifstream input_file;
    input_file.open(name, ios::in);

    Graph* graph;

    if(input_file.is_open()){
        //graph = leitura(input_file, selecao[0], selecao[1], selecao[2]);
        graph = leitura(input_file, direcionado, arestaPonderada, verticePonderado);
        input_file.close();
        saida(output_file, graph, name);
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
    cout << "Funcionalidades da parte 1 do trabalho:" << endl;
    cout << "[1] Grafo interseção" << endl;
    cout << "[2] Grafo União" << endl;
    cout << "[3] Grafo Diferença" << endl;
    cout << "[4] Rede Pert" << endl;
    cout << "Funcionalidades Auxiliares" << endl;
    cout << "[5] Imprime Grafo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){
    
    switch (selecao) {

        //Grafo Interseção
        case 1:
        {
            cout << "--------Grafo Interseção--------" << endl;
            Graph* graph2 = criaGrafo(output_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph* graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphIntersection(graph2);
            graph3->imprimeGrafo();
            graph->imprimeGrafo();
            contInterseção++;
            string nome = "Interseção" + std::to_string(contInterseção);
            saida(output_file, graph3, nome);
            delete graph3;
            delete graph2;
            break;

        }

        //Grafo União
        case 2:
        {
            cout << "--------Grafo União--------" << endl;
            Graph *graph2 = criaGrafo(output_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph *graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphUnion(graph2);
            contUniao++;
            string nome = "União" + std::to_string(contUniao);
            saida(output_file, graph3, nome);
            delete graph3;
            delete graph2;
            break;   
        }

        //Grafo Diferença
        case 3:
        {
            cout << "--------Grafo Diferença--------" << endl;
            Graph* graph2 = criaGrafo(output_file, graph->getDirected(), graph->getWeightedEdge(), graph->getWeightedNode());
            Graph* graph3 = new Graph(0, false, false, false);
            graph3 = graph->graphDiference(graph2);
            graph3->imprimeGrafo();
            contDiferenca++;
            string nome = "Diferença" + std::to_string(contDiferenca);
            saida(output_file, graph3, nome);
            delete graph3;
            delete graph2;
            break;

        }

        //Rede Pert
        case 4:
        {
            //   ./execGrupo6 entradaPert.txt saida.dot 1 1 0

            ///fazendo a impressao numa saida separada
            ofstream output_pert_saida;
            contPert++;
            string nome = "Saida_PERT"+ std::to_string(contPert) + ".txt";
            output_pert_saida.open(nome, ios::out | ios::trunc);
            if(output_pert_saida.is_open()){
                graph->pert(output_pert_saida);
                saidasPert.push_back(nome);//armazena o nomde das saidas criadas
            }
            else
                cout << "Não foi possivel abrir o arquivo de saída do PERT" << endl;
            output_pert_saida.close();
            ///fazendo a impressao na saida que foi passada no terminal como parametro
            string linha;
            ifstream output_pert_entrada(nome);
            if(output_pert_entrada.is_open()){
                if(output_file.is_open()){
                    while(getline(output_pert_entrada, linha)){
                        output_file << linha << endl;
                    }
                }else
                    cout << "Não foi possivel abrir o arquivo de saída" << endl;
            }
            else
                cout << "Não foi possivel abrir o arquivo de saída do PERT" << endl;
            output_pert_entrada.close();
            break;
        }

        //imprime Grafo
        case 5:
        {
            cout << "primeiro noh: " << graph->getFirstNode()->getId() <<" | ultimo noh: " << graph->getLastNode()->getId();
            graph->imprimeGrafo();
            //system("pause");
            break;
        }

  }
}

int mainMenu(ofstream& output_file, Graph* graph, string input_file_name, string output_file_name){

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

        saida(output_file, graph, input_file_name);
        string name = "saida.dot";
        string newName;

        //--------cria a pasta
        system("rm -vr imagens");
        system("mkdir imagens");
        system(("cp ./" + output_file_name + " ./imagens").c_str());

        //--------gera imagena
        system("dot -Tpng -O saida.dot");

        ////--------renomeia as imagens e move para a pasta
        int cont = 1;
        for(auto it = listaSaidas.begin(); it != listaSaidas.end();it++,cont++){
            if(cont==1){
                newName = "mv " + name + ".png imagem_" + *it + ".png";
            }else{
                newName = "mv " + name + "." + std::to_string(cont) + ".png imagem_" + *it + ".png";
            }
            system((newName).c_str());

            newName = "mv ./imagem_" + *it + ".png ./imagens";
            system((newName).c_str());
        }

        //--------copiando saidas do PERT para a pasta
        cont = 1;
        for(auto it = saidasPert.begin(); it != saidasPert.end();it++,cont++){
            newName = "mv ./" + *it + " ./imagens";
            system((newName).c_str());
        }
        //--------abrindo a pasta
        system("open imagens");
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
    string output_file_name(argv[2]);

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

    
    mainMenu(output_file, graph, input_file_name, output_file_name);


    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    delete graph;

    return 0;
}

