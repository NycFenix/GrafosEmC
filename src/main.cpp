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

using namespace std;

int numeroVerticesGrafo;

struct Grafo {
    int destino;
    double peso;
    Grafo(int d, double p) : destino(d), peso(p) {}
};

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
            }
        }
    }

    resultadosArquivo << "------------------------------------------" << endl;
    resultadosArquivo << "Distâncias mínimas a partir do vértice " << origem << " (usando heap):\n";
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

void dijkstra(const vector<vector<Grafo>> &grafo, int origem, std::ofstream &resultadosArquivo) {
    vector<double> distancias(numeroVerticesGrafo, numeric_limits<double>::max());
    set<int> naoVisitados;

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
            }
        }
    }

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

int main() {
    locale::global(locale("C"));
    ifstream arquivo("/home/karen/Documentos/Teoria dos grafos/GrafosEmC/estudo de caso/grafo_W_5.txt");
    bool pesoNegativoEncontrado = false;

    if (arquivo.is_open()) {
        string linha;
        getline(arquivo, linha);
        numeroVerticesGrafo = stoi(linha) + 1;

        vector<vector<Grafo>> grafo(numeroVerticesGrafo);

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

            grafo[origem].emplace_back(destino, peso);
            grafo[destino].emplace_back(origem, peso); // Se o grafo for não direcionado
        }

        arquivo.close();

        int escolha;
        cout << "DISTANCIA MINIMA:" << endl;
        cout << "Escolha a forma que deseja executar o algoritmo de Dijkstra:" << endl;
        cout << "1. Dijkstra Padrão" << endl;
        cout << "2. Dijkstra com Heap" << endl;
        cin >> escolha;

        string nomeArquivoResultado;
        if (escolha == 1) {
            nomeArquivoResultado = "resultadoSemHeap5.txt";
        } else if (escolha == 2) {
            nomeArquivoResultado = "resultadoComHeap5.txt";
        } else {
            cerr << "Escolha inválida. Nenhum algoritmo foi executado." << endl;
            return 1;
        }

        ofstream resultadosArquivo(nomeArquivoResultado);

        if (resultadosArquivo.is_open()) {
            if (escolha == 1 || escolha == 2) {
                if (pesoNegativoEncontrado) {
                    resultadosArquivo << "Aviso: A biblioteca ainda não implementa caminhos mínimos com pesos negativos." << endl;
                } else {
                    resultadosArquivo << "Nenhum peso negativo encontrado no grafo." << endl;
                    int origemDijkstra = 1;

                    // Medir o tempo de execução
                    auto start_time = chrono::high_resolution_clock::now();
                    if (escolha == 1) {
                        dijkstra(grafo, origemDijkstra, resultadosArquivo);
                    } else {
                        dijkstraHeap(grafo, origemDijkstra, resultadosArquivo);
                    }
                    auto end_time = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
                    double seconds = duration.count() / 1000000.0; // Converter para segundos

                    resultadosArquivo << "Tempo de execução";
                    if (escolha == 2) {
                        resultadosArquivo << " (usando heap)";
                    }
                    resultadosArquivo << ": " << seconds << " segundos" << endl;
                }
            } else {
                cerr << "Escolha inválida. Nenhum algoritmo foi executado." << endl;
            }

            resultadosArquivo.close();
        } else {
            cerr << "Erro ao criar o arquivo de resultados." << endl;
        }
    } else {
        cerr << "Erro ao abrir o arquivo." << endl;
    }

    cout << "O programa foi executado com sucesso.'." << endl;
    return 0;
}


// função que recebe o vetor de distâncias e o autor desejado e retorna a posição do autor no vetor

double DistanciaPesquisadores(vector<double> distancias, string autor) {
    locale::global(locale("C"));
    ifstream arquivoAutores;
    arquivoAutores.open("/home/karen/Documentos/Teoria dos grafos/GrafosEmC/estudo de caso/rede_colaboracao_vertices");
    size_t virgula;
    string linha;

    while(getline(arquivoAutores, linha)) {

        // Separa o vertice e o autor
        string vertice = linha.substr(0, virgula = linha.find(","));
        string nome = linha.substr(virgula + 1);

        //Checa se é o autor procurado
        if (nome == autor) {
            int índice = stoi(vertice);

            arquivoAutores.close();
            return distancias[índice];
        }

        arquivoAutores.close();
    }


}