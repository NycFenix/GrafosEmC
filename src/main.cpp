//Arquivo da main responsável por ler o arquivo de entrada e chamar executar as operações do grafo


#include <iostream>
#include <fstream>
using namespace std;

int main() {
    Grafo grafo;

    cout << "Digite a função que deseja executar: " << endl;
    cout << "1 - Distância entre dois vértices" << endl;
    // Botar mais opções

    cout << "6 - Distancia e caminhos mínimos" << endl;

    int opcao;

    cin >> opcao;

    switch(opcao) {
        case 1:
            cout << "Voce selecionou a opção 1" << endl;
            // ...
            break;
        case 6:
            cout << "Voce selecionou a opção 6" << endl;
            int verticeRaiz;
            cout << "Digite o vértice raiz: " << endl;
            cin >> verticeRaiz;

            grafo.dijkstra(verticeRaiz);
    }       

    




    return 0;
}