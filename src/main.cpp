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

using namespace std;

int numeroVerticesGrafo;

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

void gravarSolucao(const vector<double> &distancias, int origem, std::ofstream &resultadosArquivo) {
    resultadosArquivo << "------------------------------------------" << endl;
    resultadosArquivo << "Distâncias mínimas a partir do vértice " << origem << ":\n";
    resultadosArquivo << "------------------------------------------" << endl;

    resultadosArquivo << left << setw(10) << "Vértice" << "Distância da Origem" << endl;
    for (int i = 0; i < numeroVerticesGrafo; i++) {
        resultadosArquivo << left << setw(10) << i;
        if (distancias[i] == numeric_limits<double>::max()) {
            resultadosArquivo << "Não há caminho do vértice " << origem << " para o vértice " << i << endl;
        } else {
            resultadosArquivo << fixed << setprecision(1) << distancias[i] << endl;
        }
    }
}

void dijkstra(vector<vector<double>> &grafo, int origem, std::ofstream &resultadosArquivo) {
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
            if (grafo[u][v] > 0) {
                double distanciaTotal = distancias[u] + grafo[u][v];
                if (distanciaTotal < distancias[v]) {
                    distancias[v] = distanciaTotal;
                }
            }
        }
    }

    gravarSolucao(distancias, origem, resultadosArquivo);
}

int main() {
    std::locale::global(std::locale("C"));
    std::ifstream arquivo("entrada.txt");
    bool pesoNegativoEncontrado = false;

    if (arquivo.is_open()) {
        std::string linha;
        std::getline(arquivo, linha);
        numeroVerticesGrafo = stoi(linha) + 1;

        // matriz de adjacencia
        vector<vector<double>> grafo(numeroVerticesGrafo, vector<double>(numeroVerticesGrafo, 0.0));

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

        std::ofstream resultadosArquivo("resultados.txt");

        if (resultadosArquivo.is_open()) {
            if (pesoNegativoEncontrado) {
                resultadosArquivo << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << std::endl;
            } else {
                resultadosArquivo << "Nenhum peso negativo encontrado no grafo." << std::endl;
                int origemDijkstra = 1;

                // Medir o tempo de execução
                auto start_time = chrono::high_resolution_clock::now();
                dijkstra(grafo, origemDijkstra, resultadosArquivo);
                auto end_time = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);

                resultadosArquivo << "Tempo de execução: " << duration.count() << " microssegundos" << endl;
            }

            resultadosArquivo.close();
        } else {
            std::cerr << "Erro ao criar o arquivo de resultados." << std::endl;
        }
    } else {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    std::cout << "O programa foi executado com sucesso. Os resultados foram salvos no arquivo 'resultados.txt'." << std::endl;
    return 0;
}
