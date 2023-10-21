#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    
    std::locale::global(std::locale("C")); // Definir a localização "C" para usar o ponto como separador decimal

    std::ifstream arquivo("entrada.txt");
    bool pesoNegativoEncontrado = false;

    if (arquivo.is_open()) {
        std::string linha;

        // Ler a primeira linha que contém apenas um número
        std::getline(arquivo, linha);

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
                break;  // Parar a leitura após encontrar um peso negativo
            }
        }
        arquivo.close();

        if (pesoNegativoEncontrado) {
            std::cout << "Esta biblioteca ainda não implementa caminhos minimos com pesos negativos" << std::endl;
        } else {
            std::cout << "O grafo não possui arestas com peso negativo." << std::endl; 
            //Utilizar o algoritmo de dijkastra para encontrar a distancia de um vertice qualquer para todos os outros vertices do grafo
            //Utilizar o algoritmo de dijkastra para encontrar os caminhos minimos (arvore geradora induzida pela busca)
        }
    } else {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
    }

    return 0;
}
