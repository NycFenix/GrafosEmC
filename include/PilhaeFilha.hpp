#include <iostream>
using std::string;

// Esse header define as classes Pilha e Fila, estruturas de dados usadas no resto do código





//struct Nodo usado para Pilha e Fila
struct Nodo {
    int data;
    Nodo* next;
    Nodo(int valor): data(valor), next(nullptr) {}

};


class Pilha{
    private:
        Nodo* topo;
        int tamanho;

    public:

    Pilha(): topo(nullptr), tamanho(0) {}

    ~Pilha() {
        while (!eVazia()) {
            desempilha();
        }
    }

    bool eVazia() {
        return topo == nullptr;
    }

    void desempilha() {
        if (eVazia()) {
            throw "A pilha está vazia";
        }

        Nodo* temp = topo;
        topo = topo->next;
        tamanho--;
        delete temp;
    }

    void empilha(int valor) {
        Nodo* novo = new Nodo(valor);
        novo->next = topo;
        topo = novo;
        tamanho++;
    }

    int getTamanho() {
        return tamanho;
    }

    string toString() {
        string saida = "[ ";
        Nodo* temp = topo;

        while (temp != nullptr) {
            saida += std::to_string(temp->data) + " ";
            temp = temp->next;
        }

        saida += "]";
        return saida;
    }

};



class Fila {
    private:
        Nodo* frente;
        Nodo* tras;
        int tamanho;

    public:

    Fila(): frente(nullptr), tras(nullptr), tamanho(0) {}

    ~Fila() {
        while (!eVazia()) {
            desenfileira();
        }
    }


    //Verifica se a fila está vazia
    bool eVazia() {
        return frente == nullptr;
    }

    int getTamanho() {
        return tamanho;
    }

    //Retorna o elemento do topo da fila e retira ele da fila
    void desenfileira() {
        if (eVazia()) {
            throw "A fila está vazia";
        }

        Nodo* temp = frente;
        frente = frente->next;
        tamanho--;
        delete temp;
    }
    // Enfia elementos na fila
    void enfileira(int valor) {
        Nodo* novo = new Nodo(valor);

        if (eVazia()) {
            frente = novo;
        } else {
            tras->next = novo;
        }

        tras = novo;
        tamanho++;
    }

    //Represetação em string da Fila
    string toString() {
        string saida = "[ ";
        Nodo* temp = frente;

        while (temp != nullptr) {
            saida += std::to_string(temp->data) + " ";
            temp = temp->next;
        }

        saida += "]";
        return saida;
    }
};