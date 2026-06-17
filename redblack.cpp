#include <iostream>
using namespace std;
#include "redblack.h"

enum Color { ROJO, NEGRO };

class Node {
public:
    int dato;
    Color color;
    Node *izquierda, *derecha, *padre;

    Node(int valor) {
        dato = valor;
        color = ROJO;  // siempre se inserta rojo
        izquierda = derecha = padre = nullptr;
    }
};

class RedBlackTree {
private:
    Node* raiz;

    void rotarIzquierda(Node*& raiz, Node*& x) {
        Node* y = x->derecha;
        x->derecha = y->izquierda;
        if (y->izquierda != nullptr)
            y->izquierda->padre = x;

        y->padre = x->padre;

        if (x->padre == nullptr)
            raiz = y;
        else if (x == x->padre->izquierda)
            x->padre->izquierda = y;
        else
            x->padre->derecha = y;

        y->izquierda = x;
        x->padre = y;
    }

    void rotarDerecha(Node*& raiz, Node*& y) {
        Node* x = y->izquierda;
        y->izquierda = x->derecha;
        if (x->derecha != nullptr)
            x->derecha->padre = y;

        x->padre = y->padre;

        if (y->padre == nullptr)
            raiz = x;
        else if (y == y->padre->izquierda)
            y->padre->izquierda = x;
        else
            y->padre->derecha = x;

        x->derecha = y;
        y->padre = x;
    }

    void fixInsercion(Node*& raiz, Node*& nodo) {
        Node* padre = nullptr;
        Node* abuelo = nullptr;

        while (nodo != raiz && nodo->color == ROJO && nodo->padre->color == ROJO) {
            padre = nodo->padre;
            abuelo = padre->padre;

            // Padre es hijo izquierdo del abuelo
            if (padre == abuelo->izquierda) {
                Node* tio = abuelo->derecha;

                // Caso 1: el tío es rojo
                if (tio != nullptr && tio->color == ROJO) {
                    abuelo->color = ROJO;
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    nodo = abuelo;
                } else {
                    // Caso 2: el nodo es hijo derecho
                    if (nodo == padre->derecha) {
                        rotarIzquierda(raiz, padre);
                        nodo = padre;
                        padre = nodo->padre;
                    }
                    // Caso 3: el nodo es hijo izquierdo
                    rotarDerecha(raiz, abuelo);
                    swap(padre->color, abuelo->color);
                    nodo = padre;
                }
            }
            // Padre es hijo derecho del abuelo
            else {
                Node* tio = abuelo->izquierda;

                if (tio != nullptr && tio->color == ROJO) {
                    abuelo->color = ROJO;
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    nodo = abuelo;
                } else {
                    if (nodo == padre->izquierda) {
                        rotarDerecha(raiz, padre);
                        nodo = padre;
                        padre = nodo->padre;
                    }
                    rotarIzquierda(raiz, abuelo);
                    swap(padre->color, abuelo->color);
                    nodo = padre;
                }
            }
        }

        raiz->color = NEGRO;
    }

    void insertarNodo(Node*& raiz, Node*& nodo) {
        if (raiz == nullptr) {
            raiz = nodo;
        } else if (nodo->dato < raiz->dato) {
            insertarNodo(raiz->izquierda, nodo);
            raiz->izquierda->padre = raiz;
        } else {
            insertarNodo(raiz->derecha, nodo);
            raiz->derecha->padre = raiz;
        }
    }

    void inorden(Node* nodo) {
        if (nodo == nullptr) return;
        inorden(nodo->izquierda);
        cout << nodo->dato << (nodo->color == ROJO ? "R" : "N") << " ";
        inorden(nodo->derecha);
    }

public:
    RedBlackTree() {
        raiz = nullptr;
    }

    void insertar(int valor) {
        Node* nuevo = new Node(valor);
        insertarNodo(raiz, nuevo);
        fixInsercion(raiz, nuevo);
    }

    void mostrarInorden() {
        inorden(raiz);
        cout << endl;
    }
};