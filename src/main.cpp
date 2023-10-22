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
vector<vector<pair<int, double>>> listaAdjacencia;

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

void imprimirSolucao(const vector<double> &distancias, const vector<int> &antecessores, int origem) {
    cout << "Distâncias e caminhos mínimos a partir do vértice " << origem << ":\n";

    for (int i = 1; i < numeroVerticesGrafo; i++) {
        if (i == origem)
            continue;

        cout << origem << " para " << i << ": ";
        int verticeAtual = i;
        vector<int> caminho;
        caminho.push_back(verticeAtual);

        while (antecessores[verticeAtual] != -1) {
            caminho.push_back(antecessores[verticeAtual]);
            verticeAtual = antecessores[verticeAtual];
        }

        for (int j = caminho.size() - 1; j >= 0; j--) {
            cout << caminho[j];
            if (j > 0) {
                cout << " -> ";
            }
        }

        cout << " (Distância: " << fixed << setprecision(1) << distancias[i] << ")" << endl;
    }
}

void dijkstraMatrizAdjacencia(int origem) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    vector<int> antecessores(numeroVerticesGrafo, -1);
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
                    antecessores[v] = u;
                }
            }
        }
    }

    imprimirSolucao(distancias, antecessores, origem);
}

void dijkstraListaAdjacencia(int origem) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    vector<int> antecessores(numeroVerticesGrafo, -1);
    set<int> naoVisitados;

    distancias[origem] = 0;

    for (int i = 0; i < numeroVerticesGrafo; i++) {
        naoVisitados.insert(i);
    }

    while (!naoVisitados.empty()) {
        int u = encontrarVerticeMenorDistancia(distancias, naoVisitados);
        naoVisitados.erase(u);

        for (const pair<int, double> &vizinho : listaAdjacencia[u]) {
            int v = vizinho.first;
            double peso = vizinho.second;

            double distanciaTotal = distancias[u] + peso;
            if (distanciaTotal < distancias[v]) {
                distancias[v] = distanciaTotal;
                antecessores[v] = u;
            }
        }
    }

    imprimirSolucao(distancias, antecessores, origem);
}

int main() {
    std::locale::global(std::locale("C"));
    ifstream arquivo("entrada.txt");
    bool pesoNegativoEncontrado = false;

    if (arquivo.is_open()) {
        string linha;
        getline(arquivo, linha);
        numeroVerticesGrafo = stoi(linha) + 1;

        int escolha;
        cout << "Escolha a representação do grafo (1 - Matriz de Adjacência, 2 - Lista de Adjacência): ";
        cin >> escolha;

        if (escolha == 1) {
            matrizAdjacencia.assign(numeroVerticesGrafo, vector<double>(numeroVerticesGrafo, 0.0));
            while (getline(arquivo, linha)) {
                istringstream iss(linha);
                int origem, destino;
                double peso;

                if (!(iss >> origem >> destino >> peso)) {
                    cerr << "Erro ao ler o arquivo." << endl;
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
            while (getline(arquivo, linha)) {
                istringstream iss(linha);
                int origem, destino;
                double peso;

                if (!(iss >> origem >> destino >> peso)) {
                    cerr << "Erro ao ler o arquivo." << endl;
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
            cout << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << endl;
        } else {
            cout << "Nenhum peso negativo encontrado no grafo." << endl;
            int origemDijkstra = 1;

            if (escolha == 1) {
                cout << "------------------------------------------" << endl;
                cout << "Distância e caminho mínimo a partir do vértice " << origemDijkstra << " usando matriz de adjacência:\n";
                cout << "------------------------------------------" << endl;
                dijkstraMatrizAdjacencia(origemDijkstra);
            } else if (escolha == 2) {
                cout << "Distância e caminho mínimo a partir do vértice " << origemDijkstra << " usando lista de adjacência:\n";
                dijkstraListaAdjacencia(origemDijkstra);
            }
        }
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
    }

    return 0;
}
