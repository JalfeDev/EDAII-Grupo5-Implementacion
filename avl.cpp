#include "avl.h"

template <typename T>
class AVLNode {
public:
  T key;
  AVLNode* left;
  AVLNode* right;
  int height;

  AVLNode(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

template <typename T>
int height(AVLNode<T>* nodo) {
  return nodo ? nodo->height : 0;
}

template <typename T>
int balanceFactor(AVLNode<T>* nodo) {
  return nodo ? height(nodo->left) - height(nodo->right) : 0;
}

template <typename T>
AVLNode<T>* rotateRight(AVLNode<T>* y) {
  AVLNode<T>* x = y->left;
  AVLNode<T>* T2 = x->right;
  x->right = y;
  y->left = T2;
  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;
  return x;
}

template <typename T>
AVLNode<T>* rotateLeft(AVLNode<T>* x) {
  AVLNode<T>* y = x->right;
  AVLNode<T>* T2 = y->left;
  y->left = x;
  x->right = T2;
  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;
  return y;
}

class AVLTree {
private:
  AVLNode<Transaccion*>* raiz;

  bool esMenor(Transaccion* a, Transaccion* b) {
    return fechaHoraCompleta(*a) < fechaHoraCompleta(*b);
  }

  AVLNode<Transaccion*>* insertarNodo(AVLNode<Transaccion*>* nodo, Transaccion* t) {
    if (!nodo) return new AVLNode<Transaccion*>(t);

    if (esMenor(t, nodo->key))
      nodo->left = insertarNodo(nodo->left, t);
    else
      nodo->right = insertarNodo(nodo->right, t);

    nodo->height = 1 + max(height(nodo->left), height(nodo->right));
    int bf = balanceFactor(nodo);

    if (bf > 1 && esMenor(t, nodo->left->key)) return rotateRight(nodo);
    if (bf < -1 && !esMenor(t, nodo->right->key)) return rotateLeft(nodo);
    if (bf > 1 && !esMenor(t, nodo->left->key)) {
      nodo->left = rotateLeft(nodo->left);
      return rotateRight(nodo);
    }
    if (bf < -1 && esMenor(t, nodo->right->key)) {
      nodo->right = rotateRight(nodo->right);
      return rotateLeft(nodo);
    }
    return nodo;
  }

  AVLNode<Transaccion*>* minimoNodo(AVLNode<Transaccion*>* nodo) {
    AVLNode<Transaccion*>* actual = nodo;
    while (actual && actual->left) actual = actual->left;
    return actual;
  }

  AVLNode<Transaccion*>* eliminarNodo(AVLNode<Transaccion*>* nodo, Transaccion* objetivo) {
    if (!nodo) return nullptr;

    if (nodo->key->idTransaccion == objetivo->idTransaccion) {
      if (!nodo->left || !nodo->right) {
        AVLNode<Transaccion*>* hijo = nodo->left ? nodo->left : nodo->right;
        delete nodo;
        return hijo;
      } else {
        AVLNode<Transaccion*>* sucesor = minimoNodo(nodo->right);
        nodo->key = sucesor->key;
        nodo->right = eliminarNodo(nodo->right, sucesor->key);
      }
    } else if (esMenor(objetivo, nodo->key)) {
      nodo->left = eliminarNodo(nodo->left, objetivo);
    } else {
      nodo->right = eliminarNodo(nodo->right, objetivo);
    }

    if (!nodo) return nullptr;

    nodo->height = 1 + max(height(nodo->left), height(nodo->right));
    int bf = balanceFactor(nodo);

    if (bf > 1 && balanceFactor(nodo->left) >= 0) return rotateRight(nodo);
    if (bf > 1 && balanceFactor(nodo->left) < 0) {
      nodo->left = rotateLeft(nodo->left);
      return rotateRight(nodo);
    }
    if (bf < -1 && balanceFactor(nodo->right) <= 0) return rotateLeft(nodo);
    if (bf < -1 && balanceFactor(nodo->right) > 0) {
      nodo->right = rotateRight(nodo->right);
      return rotateLeft(nodo);
    }
    return nodo;
  }

  void inorden(AVLNode<Transaccion*>* nodo, int& contador, int limite) {
    if (!nodo || contador >= limite) return;
    inorden(nodo->left, contador, limite);
    if (contador < limite) {
      nodo->key->mostrar();
      contador++;
    }
    inorden(nodo->right, contador, limite);
  }

  void inordenRango(AVLNode<Transaccion*>* nodo, const string& fechaInicio, const string& fechaFin, int& contador) {
    if (!nodo) return;
    string fh = fechaHoraCompleta(*nodo->key);

    if (fh > fechaInicio) inordenRango(nodo->left, fechaInicio, fechaFin, contador);
    if (fh >= fechaInicio && fh <= fechaFin) {
      nodo->key->mostrar();
      contador++;
    }
    if (fh < fechaFin) inordenRango(nodo->right, fechaInicio, fechaFin, contador);
  }

  void liberar(AVLNode<Transaccion*>* nodo) {
    if (!nodo) return;
    liberar(nodo->left);
    liberar(nodo->right);
    delete nodo;
  }

public:
  AVLTree() : raiz(nullptr) {}
  ~AVLTree() { liberar(raiz); }

  void insertar(Transaccion* t) { raiz = insertarNodo(raiz, t); }
   
  bool eliminar(Transaccion* objetivo) {
    if (!objetivo) return false;
    raiz = eliminarNodo(raiz, objetivo);
    return true;
  }

  void mostrarInorden(int limite = 50) {
    int contador = 0;
    inorden(raiz, contador, limite);
    cout << "Se mostraron " << contador << " transacciones cronologicamente.\n";
  }

  int consultarRango(const string& fechaInicio, const string& fechaFin) {
    int contador = 0;
    inordenRango(raiz, fechaInicio, fechaFin + " 23:59:59", contador);
    cout << "Transacciones encontradas en el rango: " << contador << "\n";
    return contador;
  }
};
