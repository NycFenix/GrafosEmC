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

int numeroVerticesGrafo;
vector<vector<double>> matrizAdjacencia;
vector<vector<pair<int, double>> > listaAdjacencia;

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

void imprimirSolucao(const vector<double> &distancias, int origem) {
    cout << "------------------------------------------" << endl;
    cout << "Distâncias mínimas a partir do vértice " << origem << ":\n";
    cout << "------------------------------------------" << endl;

    cout << left << setw(10) << "Vértice" << "Distância da Origem" << endl;
    for (int i = 0; i < numeroVerticesGrafo; i++) {
        cout << left << setw(10) << i;
        if (distancias[i] == numeric_limits<double>::max()) {
            cout << "Não há caminho do vértice " << origem << " para o vértice " << i << endl;
        } else {
            cout << fixed << setprecision(1) << distancias[i] << endl;
        }
    }
}

void dijkstraMatrizAdjacencia(int origem) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    set<int> naoVisitados;

    distancias[origem] = 0;

    for (int i = 0; i < numeroVerticesGrafo; i++) {
        naoVisitados.insert(i);
    }

    while (!naoVisitados.empty()) {
        int u = encontrarVerticeMenorDistancia(distancias, naoVisitados);
        naoVisitados.erase(u);

        for (int v = 0; v < numeroVerticesGrafo; v++) {
            if (matrizAdjacencia[u][v] > 0) {
                double distanciaTotal = distancias[u] + matrizAdjacencia[u][v];
                if (distanciaTotal < distancias[v]) {
                    distancias[v] = distanciaTotal;
                }
            }
        }
    }

    imprimirSolucao(distancias, origem);
}

void dijkstraListaAdjacencia(int origem) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    set<int> naoVisitados;

    distancias[origem] = 0;

    for (int i = 0; i < numeroVerticesGrafo; i++) {
        naoVisitados.insert(i);
    }

    while (!naoVisitados.empty()) {
        int u = encontrarVerticeMenorDistancia(distancias, naoVisitados);
        naoVisitados.erase(u);

        for (const pair<int, double>& vizinho : listaAdjacencia[u]) {
            int v = vizinho.first;
            double peso = vizinho.second;

            double distanciaTotal = distancias[u] + peso;
            if (distanciaTotal < distancias[v]) {
                distancias[v] = distanciaTotal;
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
        numeroVerticesGrafo = stoi(linha) + 1;

        int escolha;
        cout << "Escolha a representação do grafo (1 - Matriz de Adjacência, 2 - Lista de Adjacência): ";
        cin >> escolha;

        if (escolha == 1) {
            matrizAdjacencia.assign(numeroVerticesGrafo, vector<double>(numeroVerticesGrafo, 0.0));
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

                matrizAdjacencia[origem][destino] = peso;
                matrizAdjacencia[destino][origem] = peso;
            }
        } else if (escolha == 2) {
            listaAdjacencia.assign(numeroVerticesGrafo, vector<pair<int, double>>());
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

                listaAdjacencia[origem].push_back({destino, peso});
                listaAdjacencia[destino].push_back({origem, peso});
            }
        } else {
            cout << "Escolha inválida." << endl;
            return 1;
        }

        arquivo.close();

        cout << "Algoritmo de Dijkstra para encontrar caminhos mínimos em um grafo." << endl;

        if (pesoNegativoEncontrado) {
            std::cout << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << std::endl;
        } else {
            std::cout << "Nenhum peso negativo encontrado no grafo." << std::endl;
            int origemDijkstra = 1;

            if (escolha == 1) {
                cout << "Distâncias mínimas a partir do vértice " << origemDijkstra << " usando matriz de adjacência:\n";
                dijkstraMatrizAdjacencia(origemDijkstra);
            } else if (escolha == 2) {
                cout << "Distâncias mínimas a partir do vértice " << origemDijkstra << " usando lista de adjacência:\n";
                dijkstraListaAdjacencia(origemDijkstra);
            }
        }
    } else {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    return 0;
}
