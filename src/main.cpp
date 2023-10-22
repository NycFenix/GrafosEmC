#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <climits>
#include <iomanip>
#include <limits>
#include <set>

using namespace std;

int V = 10001;

int encontrarVerticeMenorDistancia(const vector<double> &distancias, const set<int> &naoVisitados) {
    double menorDistancia = numeric_limits<double>::max();
    int verticeMenorDistancia = -1;

    for (int v = 0; v < V; v++) {
        if (naoVisitados.count(v) && distancias[v] <= menorDistancia) {
            menorDistancia = distancias[v]; //distancias - armazena as estimativas de distancia pra cada vertice
            verticeMenorDistancia = v;
        }
    }

    return verticeMenorDistancia;
}

void imprimirSolucao(const vector<double> &distancias, int origem) {
    cout << "------------------------------------------" << endl;
    cout << "Distâncias mínimas a partir do vértice " << origem << ":\n";
    cout << "------------------------------------------" << endl;

    cout << left << setw(10) << "Vértice" << "Distância da Origem" << endl;
    for (int i = 0; i < V; i++) {
        cout << left << setw(10) << i;
        if (distancias[i] == numeric_limits<double>::max()) {
            cout << "Não há caminho do vértice " << origem << " para o vértice " << i << endl;
        } else {
            cout << fixed << setprecision(1) << distancias[i] << endl;
        }
    }
}

void dijkstra(vector<vector<double>> &grafo, int origem) {
    vector<double> distancias(V, numeric_limits<double>::max());
    set<int> naoVisitados;

    distancias[origem] = 0;

    for (int i = 0; i < V; i++) {
        naoVisitados.insert(i);
    }

    while (!naoVisitados.empty()) {
        int u = encontrarVerticeMenorDistancia(distancias, naoVisitados);
        naoVisitados.erase(u);

        for (int v = 0; v < V; v++) {
            if (grafo[u][v] > 0) {
                double distanciaTotal = distancias[u] + grafo[u][v];
                if (distanciaTotal < distancias[v]) {
                    distancias[v] = distanciaTotal;
                }
            }
        }
    }

    imprimirSolucao(distancias, origem);
}

int main() {
    std::locale::global(std::locale("C"));
    std::ifstream arquivo("grafo_W_1.txt");
    bool pesoNegativoEncontrado = false;

    if (arquivo.is_open()) {
        std::string linha;
        std::getline(arquivo, linha);
        int numArestas = stoi(linha);

        vector<vector<double>> grafo(V, vector<double>(V, 0.0));

        while (std::getline(arquivo, linha)) {
            std::istringstream iss(linha);
            int origem, destino;
            double peso;

            if (!(iss >> origem >> destino >> peso)) {
                std::cerr << "Erro ao ler o arquivo." << std::endl;
                return 1;
            }

            if (peso < 0) {
                pesoNegativoEncontrado = true;
                break;
            }

            grafo[origem][destino] = peso;
            grafo[destino][origem] = peso;
        }
        arquivo.close();

        cout << "Algoritmo de Dijkstra para encontrar caminhos mínimos em um grafo." << endl;

        if (pesoNegativoEncontrado) {
            std::cout << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << std::endl;
        } else {
            std::cout << "Nenhum peso negativo encontrado no grafo." << std::endl;
            int origemDijkstra = 1;
            cout << "Distâncias mínimas a partir do vértice " << origemDijkstra << ":\n";
            dijkstra(grafo, origemDijkstra);
        }
    } else {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    return 0;
}
