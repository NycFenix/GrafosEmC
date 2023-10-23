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
#include <algorithm> // Adicione esta linha para incluir a biblioteca de algoritmos.
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

// Função para encontrar o caminho mínimo entre 2 vertices com base nas informações de distâncias mínimas do algoritmo de Dijkstra
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

void dijkstraHeap(const vector<vector<Grafo>> &grafo, int origem, std::ofstream &resultadosArquivo) {

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

    resultadosArquivo << "------------------------------------------" << endl;
    resultadosArquivo << "Distâncias/caminho mínimos a partir do vértice " << origem << " (usando heap):\n";
    resultadosArquivo << "------------------------------------------" << endl;

    resultadosArquivo << left << setw(10) << "Vértice" << setw(25) << "Distância da Origem" << "Caminho Mínimo" << endl;
    for (int i = 0; i < numeroVerticesGrafo; i++) {
        resultadosArquivo << left << setw(10) << i;
        if (distancias[i] == numeric_limits<double>::max()) {
            resultadosArquivo << setw(25) << "N/A" << endl;
        } else {
            resultadosArquivo << fixed << setw(25) << setprecision(1) << distancias[i];
        }
    }
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








int main() {

    locale::global(locale("C"));
    ifstream arquivo("entrada.txt");

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
        //SEM HEAP - ORIGEM e DESTINO DEFINIDOS - APENAS DISTANCIA
        ofstream resultados1ArquivoDistancia("Estudo1DistanciaSemHeap.txt");

        vector<int> destinos = {2, 3, 4, 5};
        int origem = 1; // vértice de origem

        
        auto tempo_inicio_distancia = chrono::high_resolution_clock::now();
        resultados1ArquivoDistancia << "------------------------------------------" << endl;
        resultados1ArquivoDistancia << "Origem Vértice " << origem << ":\n";
        resultados1ArquivoDistancia << "------------------------------------------" << endl;
        resultados1ArquivoDistancia << left << setw(10) << "Vértice" << setw(25) << "Distância"<< endl;
        for (int destino : destinos) {
            DijkstraResult resultado = dijkstra(grafo, origem);
            resultados1ArquivoDistancia << left << setw(10) << destino;
            if (resultado.distancias[destino] == numeric_limits<double>::max()) {
                resultados1ArquivoDistancia << setw(25) << "N/A" << endl;
            } else {
                resultados1ArquivoDistancia << fixed << setw(25) << setprecision(1) << resultado.distancias[destino];
            resultados1ArquivoDistancia << endl;
            }
        }
        auto tempo_final_distancia = chrono::high_resolution_clock::now();
        auto duracao = chrono::duration_cast<chrono::microseconds>(tempo_final_distancia - tempo_inicio_distancia);
        double segundos = duracao.count() / 1000000.0;
        resultados1ArquivoDistancia << "Tempo de execução: " << segundos << " segundos" << endl;

        // //=======================================================================================
        //SEM HEAP - ORIGEM e DESTINO DEFINIDOS - APENAS CAMINHO

        ofstream resultadosArquivoCaminho("Estudo1CaminhoSemHeap.txt");
        DijkstraResult resultado = dijkstra(grafo, origem);
        resultadosArquivoCaminho << "------------------------------------------" << endl;
        resultadosArquivoCaminho << "Origem Vértice " << origem << ":\n";
        resultadosArquivoCaminho << "------------------------------------------" << endl;
        resultadosArquivoCaminho << left << setw(10) << "Vértice" << setw(25) << "Caminho" << endl;
        for (int destino : destinos) {
            vector<int> caminhoMinimo = encontrarCaminhoMinimo(resultado.pais, origem, destino);
            resultadosArquivoCaminho << left << setw(10) << destino;
            if (caminhoMinimo.empty()) {
                resultadosArquivoCaminho << setw(25) << "N/A" << endl;
            } else {
                for (int i = 0; i < caminhoMinimo.size(); i++) {
                    resultadosArquivoCaminho << caminhoMinimo[i];
                    if (i < caminhoMinimo.size() - 1) {
                        resultadosArquivoCaminho << "->";
                    }
                }
                resultadosArquivoCaminho << endl;
            }
        }

        //=======================================================================================
        //SEM HEAP - TODOS OS VERTICES - APENAS DISTANCIA
        ofstream arquivo2("Estudo2DistanciaSemHeap.txt");

        double segundos_totais;

        // Inicializa o gerador de números aleatórios com uma semente
        std::random_device rd;
        std::mt19937 gen(rd());

        // Define o intervalo
        int min = 1;
        int max = numeroVerticesGrafo-1;

        // Crie uma distribuição uniforme dentro do intervalo
        std::uniform_int_distribution<int> distribution(min, max);

        //Gera números aleatórios dentro do intervalo
        for (int i = 1; i < numeroVerticesGrafo/2; i++) {
            int random_number = distribution(gen);
            std::cout << "Número aleatório " << i+1 << ": " << random_number << std::endl;

            auto tempo2_inicio_distancia = chrono::high_resolution_clock::now();
            arquivo2 << "------------------------------------------" << endl;
            arquivo2 << "Origem Vértice " << random_number << ":\n";
            arquivo2 << "------------------------------------------" << endl;
            arquivo2 << left << setw(10) << "Vértice" << setw(25) << "Distância"<< endl;
            
            DijkstraResult resultados = dijkstra(grafo, random_number);
        
            arquivo2 << "------------------------------------------" << endl;
            arquivo2 << "Origem Vértice " << random_number << ":\n";
            arquivo2 << "------------------------------------------" << endl;

            arquivo2 << left << setw(10) << "Vértice" << setw(25) << "Distância da Origem"<< endl;
            for (int i = 1; i < numeroVerticesGrafo; i++) {
                arquivo2 << left << setw(10) << i;
                if (resultados.distancias[i] == numeric_limits<double>::max()) {
                    arquivo2 << setw(25) << "N/A" << endl;
                } else {
                    arquivo2 << fixed << setw(25) << setprecision(1) << resultados.distancias[i];
                    arquivo2 << endl;
                }
            }
            
            auto tempo_final_distancia2 = chrono::high_resolution_clock::now();
            auto duracao2 = chrono::duration_cast<chrono::seconds>(tempo_final_distancia2 - tempo2_inicio_distancia);
            segundos_totais += duracao2.count(); 
            arquivo2 << "Tempo de execução: " << segundos_totais << " segundos" << endl;
        }
        arquivo2 << "Tempo total de execução: " << segundos_totais << " segundos" << endl;
        double media_amostral = segundos_totais / numeroVerticesGrafo/2;
        arquivo2 << "Média Amostral: " << media_amostral << " segundos por amostra" << std::endl;
        std::cout << "Média Amostral: " << media_amostral << " segundos por amostra" << std::endl;
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
    }
    cout << "O programa foi executado com sucesso." << endl;
    return 0;
}