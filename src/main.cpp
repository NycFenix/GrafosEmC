#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <limits>
#include <set>
#include <chrono>
#include <queue>
#include <algorithm>
#include <random>

using namespace std;


struct DijkstraResult {
    vector<double> distancias;
    vector<int> pais;
};

int numeroVerticesGrafo;

struct Grafo {
    int destino;
    double peso;
    Grafo(int d, double p) : destino(d), peso(p) {}
};

vector<int> encontrarCaminhoMinimo(const vector<int> &pais, int origem, int destino) {
    vector<int> caminho;
    int atual = destino;

    while (atual != -1 && atual != origem) {
        caminho.push_back(atual);
        atual = pais[atual];
    }

    if (atual == origem) {
        caminho.push_back(origem);
        reverse(caminho.begin(), caminho.end());
    }

    return caminho;
}

int encontrarVerticeMenorDistancia(const vector<double> &distancias, const set<int> &naoVisitados) {
    double menorDistancia = numeric_limits<double>::max();
    int verticeMenorDistancia = -1;

    for (int v = 0; v < numeroVerticesGrafo; v++) {
        if (naoVisitados.count(v) && distancias[v] <= menorDistancia) {
            menorDistancia = distancias[v];
            verticeMenorDistancia = v;
        }
    }

    return verticeMenorDistancia;
}

DijkstraResult dijkstraHeap(const vector<vector<Grafo>> &grafo, int origem) {

    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    set<int> naoVisitados;
    vector<int> pais(numeroVerticesGrafo, -1);  // Para rastrear os pais

    distancias[origem] = 0;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> minHeap;
    minHeap.push({0, origem});

    while (!minHeap.empty()) {
        int u = minHeap.top().second;
        double dist_u = minHeap.top().first;  // Distância para u
        minHeap.pop();

        if (dist_u > distancias[u]) {
            continue;  // Ignorar se uma distância mais curta já foi encontrada
        }

        naoVisitados.erase(u);

        for (const Grafo &vizinho : grafo[u]) {
            int v = vizinho.destino;
            double peso = vizinho.peso;
            double distanciaTotal = distancias[u] + peso;
            if (distanciaTotal < distancias[v]) {
                distancias[v] = distanciaTotal;
                minHeap.push({distancias[v], v});
                pais[v] = u;  // Rastrear o pai de v no caminho mais curto
            }
        }
    }


    DijkstraResult result;
    result.distancias = distancias;
    result.pais = pais;

    return result;
}

DijkstraResult dijkstra(const vector<vector<Grafo>> &grafo, int origem) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    set<int> naoVisitados;
    vector<int> pais(numeroVerticesGrafo, -1);  // Para rastrear os pais

    distancias[origem] = 0;

    for (int i = 0; i < numeroVerticesGrafo; i++) {
        naoVisitados.insert(i);
    }

    while (!naoVisitados.empty()) {
        int u = encontrarVerticeMenorDistancia(distancias, naoVisitados);
        naoVisitados.erase(u);

        for (const Grafo &vizinho : grafo[u]) {
            int v = vizinho.destino;
            double peso = vizinho.peso;
            double distanciaTotal = distancias[u] + peso;
            if (distanciaTotal < distancias[v]) {
                distancias[v] = distanciaTotal;
                pais[v] = u;  // Rastrear o pai de v no caminho mais curto
            }
        }
    }

    DijkstraResult result;
    result.distancias = distancias;
    result.pais = pais;

    return result;
}


void calcularDistanciaIntervalo(const vector<vector<Grafo>> &grafo, int origem, vector<int> destinos, const std::string& nome_arquivo, bool heap){
    ofstream arquivo(nome_arquivo);

    auto tempo_inicio_distancia_heap = chrono::high_resolution_clock::now();
    arquivo << "------------------------------------------" << endl;
    arquivo << "Origem Vértice " << origem << ":\n";
    arquivo << "------------------------------------------" << endl;

    for (int destino : destinos) {
        DijkstraResult resultado = heap ? dijkstraHeap(grafo, origem) : dijkstra(grafo, origem);
        arquivo << left << setw(10) << destino;
        if (resultado.distancias[destino] == numeric_limits<double>::max()) {
            arquivo << setw(25) << "N/A" << endl;
        } else {
            arquivo << fixed << setw(25) << setprecision(1) << resultado.distancias[destino];
        arquivo << endl;
        }
    }
    auto tempo_final_distancia_heap = chrono::high_resolution_clock::now();
    auto duracao_distancia_heap = chrono::duration_cast<chrono::microseconds>(tempo_final_distancia_heap - tempo_inicio_distancia_heap);
    double segundos_distancia_heap = duracao_distancia_heap.count() / 1000000.0;
    arquivo << "Tempo de execução: " << segundos_distancia_heap << " segundos" << endl;
    arquivo.close();

}


void calcularCaminhoIntervalo(const vector<vector<Grafo>> &grafo, int origem, vector<int> destinos, const std::string& nome_arquivo, bool heap){
    ofstream arquivo(nome_arquivo);
    DijkstraResult resultado = heap ? dijkstraHeap(grafo, origem) : dijkstra(grafo, origem);
    arquivo << "------------------------------------------" << endl;
    arquivo << "Origem Vértice " << origem << ":\n";
    arquivo << "------------------------------------------" << endl;
    arquivo << left << setw(10) << "Vértice" << setw(25) << "Caminho" << endl;
    for (int destino : destinos) {
        vector<int> caminhoMinimo = encontrarCaminhoMinimo(resultado.pais, origem, destino);
        arquivo << left << setw(10) << destino;
        if (caminhoMinimo.empty()) {
            arquivo << setw(25) << "N/A" << endl;
        } else {
            for (int i = 0; i < caminhoMinimo.size(); i++) {
                arquivo << caminhoMinimo[i];
                if (i < caminhoMinimo.size() - 1) {
                    arquivo << "->";
                }
            }
            arquivo << endl;
        }
    }
    arquivo.close();
}

void calcularDistanciaTodos(const vector<vector<Grafo>> &grafo, int origem, const std::string& nome_arquivo, bool heap){

    double tempo_execucao;
    double total;


    std::random_device rd;
    std::mt19937 gen(rd());


    int min = 1;
    int max = numeroVerticesGrafo-1;


    std::uniform_int_distribution<int> distribution(min, max);

    auto tempo_inicio_distancia = chrono::high_resolution_clock::now();

 
    for (int i = 1; i < 100; i++) {
        int random_number = distribution(gen);

        DijkstraResult resultados = heap ? dijkstraHeap(grafo, random_number) : dijkstra(grafo, random_number);

        
        auto tempo_final_distancia = chrono::high_resolution_clock::now();
        auto duracao = chrono::duration_cast<chrono::seconds>(tempo_final_distancia - tempo_inicio_distancia);
        tempo_execucao = duracao.count(); 
        total += tempo_execucao;
    }
    double media_amostral = (total / 100);
    std::cout  << "Tempo total de execução: " << total << " segundos" << endl;    
    std::cout << "Média Amostral: " << media_amostral << " segundos por amostra" << std::endl; 
}

void calcularDistanciaPesquisadores(const vector<vector<Grafo>> &grafo) {
    
    unordered_map<string, int> nomeParaIndice;
    unordered_map<int, string> indiceParaNome;

    ifstream arquivoNomes("../estudo de caso/rede_colaboracao_vertices.txt"); 

    if (arquivoNomes.is_open()) {
        string linha;
        while (getline(arquivoNomes, linha)) {
            size_t pos = linha.find(",");

            if (pos != std::string::npos) {
                int indice = std::stoi(linha.substr(0, pos));
                std::string nome = linha.substr(pos + 1);
                nomeParaIndice[nome] = indice;
                indiceParaNome[indice] = nome;
            }
        }
        arquivoNomes.close();

        int origem = nomeParaIndice["Edsger W. Dijkstra"];
        vector<string> pesquisadoresDestino = {
            "Alan M. Turing",
            "J. B. Kruskal",
            "Jon M. Kleinberg",
            "Eva Tardos",
            "Daniel R. Figueiredo"
        };

        for (const string &pesquisadorDestino : pesquisadoresDestino) {
            int destino = nomeParaIndice[pesquisadorDestino];

            DijkstraResult resultado = dijkstraHeap(grafo, origem);

            if (resultado.distancias[destino] < numeric_limits<double>::max()) {
                cout << "Distância entre " << indiceParaNome[origem] << " e " << indiceParaNome[destino] << ": " << resultado.distancias[destino] << endl;

                vector<int> caminhoMinimo = encontrarCaminhoMinimo(resultado.pais, origem, destino);
                cout << "Caminho mínimo: ";
                for (int i = 0; i < caminhoMinimo.size(); i++) {
                    cout << indiceParaNome[caminhoMinimo[i]];
                    if (i < caminhoMinimo.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;
            } else {
                cout << "Não há caminho válido entre " << indiceParaNome[origem] << " e " << indiceParaNome[destino] << endl;
            }
        }
    }
}


int main() {

    locale::global(locale("C"));

    //para estudo de caso 1 e 2 abrir os grafos disponibilizados no site, para o caso 3 abrir arquivo da rede de colaboradores
    ifstream arquivo("../estudo de caso/rede_colaboracao.txt");

    if (arquivo.is_open()) {
        string linha;
        getline(arquivo, linha);
        numeroVerticesGrafo = stoi(linha) + 1;

        vector<vector<Grafo>> grafo(numeroVerticesGrafo);

        //leitura do arquivo do grafo
        while (getline(arquivo, linha)) {
            istringstream iss(linha);
            int origem, destino;
            double peso;

            if (!(iss >> origem >> destino >> peso)) {
                cerr << "Erro ao ler o arquivo." << endl;
                return 1;
            }

            if (peso < 0) {
                std::cout << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << std::endl;
                break;
            }

            grafo[origem].emplace_back(destino, peso);
            grafo[destino].emplace_back(origem, peso); // grafo não-direcionado
        }

        arquivo.close();


        //========================================ESTUDO DE CASO========================================

        vector<int> destinos = {20, 30, 40, 50, 60};
        int origem = 10;

        calcularDistanciaIntervalo(grafo, origem, destinos, "caso1_distancia_intervalo_heap.txt",true); //com heap
        calcularCaminhoIntervalo(grafo, origem, destinos, "caso1_caminho_intervalo_heap.txt",true); //com heap
        calcularDistanciaTodos(grafo, origem, "caso2_distancia_intervalo_heap.txt",true); //com heap
        calcularDistanciaTodos(grafo, origem, "caso2_distancia_intervalo_sem_heap.txt",false); //sem heap
        calcularDistanciaPesquisadores(grafo);
        
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
    }

    cout << "O programa foi executado com sucesso." << endl;
    return 0;
}


