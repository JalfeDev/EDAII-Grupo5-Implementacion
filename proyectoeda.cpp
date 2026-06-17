#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

struct Transaccion {
    string idTransaccion;
    string cuentaOrigen;
    string cliente;
    string tipo;
    double monto;
    string fecha;
    string hora;
    string estado;

    void mostrar() const {
        cout << idTransaccion << " | " << cliente << " | " << tipo
             << " | S/." << monto << " | " << fecha << " " << hora
             << " | " << estado << "\n";
    }
};

string fechaHoraCompleta(const Transaccion& t) {
    return t.fecha + " " + t.hora;
}


class Node {
private:
    Transaccion* value;
    Node* next;
public:
    Node(Transaccion* v) {
        value = v;
        next = nullptr;
    }
    void setNext(Node* tmp) { next = tmp; }
    Node* getNext() { return next; }
    Transaccion* getValue() { return value; }
};
)
class linkedList {
private:
    Node* head;
public:
    linkedList() { head = nullptr; }


    ~linkedList() {
        Node* tmp = head;
        while(tmp != nullptr) {
            Node* aBorrar = tmp;
            tmp = tmp->getNext();
            delete aBorrar;
        }
    }


    bool insert(Transaccion* N) {

        Node* tmp = head;
        while(tmp != nullptr) {
            if (tmp->getValue()->idTransaccion == N->idTransaccion) return false;
            tmp = tmp->getNext();
        }

        Node* nodito = new Node(N);
        nodito->setNext(head);
        head = nodito;
        return true;
    }

    Transaccion* search(const string& id) {
        Node* tmp = head;
        while(tmp != nullptr) {
            if(tmp->getValue()->idTransaccion == id) return tmp->getValue();
            tmp = tmp->getNext();
        }
        return nullptr;
    }

    bool remove(const string& id) {
        Node* tmp = head;
        Node* prev = nullptr;

        while (tmp != nullptr && tmp->getValue()->idTransaccion != id) {
            prev = tmp;
            tmp = tmp->getNext();
        }

        if (tmp == nullptr) return false;

        if (prev == nullptr) {
            head = tmp->getNext();
        } else {
            prev->setNext(tmp->getNext());
        }

        delete tmp;
        return true;
    }

    Node* getHead() { return head; }
};


class hashTable {
private:
    linkedList* table;
    int size;
    int count;

public:
    hashTable(int s) {
        size = s;
        table = new linkedList[size];
        count = 0;
    }

    ~hashTable() {

        for (int i = 0; i < size; i++) {
            Node* tmp = table[i].getHead();
            while (tmp != nullptr) {
                delete tmp->getValue();
                tmp = tmp->getNext();
            }
        }
        delete[] table;
    }

    int hashFunction(const string& id) {
        unsigned long hash = 5381;
        for (int i = 0; i < (int)id.size(); i++) {
            hash = ((hash << 5) + hash) + id[i];
        }
        return hash % size;
    }

    bool insert(Transaccion* t) {
        int pos = hashFunction(t->idTransaccion);
        if (table[pos].insert(t)) {
            count++;
            return true;
        }
        return false;
    }

    Transaccion* search(const string& id) {
        int pos = hashFunction(id);
        return table[pos].search(id);
    }

    bool remove(const string& id) {
        int pos = hashFunction(id);
        if (table[pos].remove(id)) {
            count--;
            return true;
        }
        return false;
    }

    void calcularEstadisticas() {
        if (count == 0) {
            cout << "No hay transacciones registradas.\n";
            return;
        }
        double montoTotal = 0;
        double maxMonto = -1, minMonto = 999999999;
        string idMax, idMin;

        for (int i = 0; i < size; i++) {
            Node* tmp = table[i].getHead();
            while (tmp != nullptr) {
                Transaccion* tx = tmp->getValue();
                montoTotal += tx->monto;
                if (tx->monto > maxMonto) { maxMonto = tx->monto; idMax = tx->idTransaccion; }
                if (tx->monto < minMonto) { minMonto = tx->monto; idMin = tx->idTransaccion; }
                tmp = tmp->getNext();
            }
        }
        cout << "ESTADISTICAS \n";
        cout << "Total de transacciones: " << count << "\n";
        cout << "Monto total transaccionado: S/." << montoTotal << "\n";
        cout << "Monto promedio: S/." << (montoTotal / count) << "\n";
        cout << "Transaccion de MAYOR monto: " << idMax << " (S/." << maxMonto << ")\n";
        cout << "Transaccion de MENOR monto: " << idMin << " (S/." << minMonto << ")\n";
    }
};


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
int height(AVLNode<T>* nodo) { return nodo ? nodo->height : 0; }

template <typename T>
int balanceFactor(AVLNode<T>* nodo) { return nodo ? height(nodo->left) - height(nodo->right) : 0; }

template <typename T>
AVLNode<T>* rotateRight(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

template <typename T>
AVLNode<T>* rotateLeft(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* T2 = y->left;
    y->left = x; x->right = T2;
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

        if (esMenor(t, nodo->key)) nodo->left = insertarNodo(nodo->left, t);
        else nodo->right = insertarNodo(nodo->right, t);

        nodo->height = 1 + max(height(nodo->left), height(nodo->right));
        int bf = balanceFactor(nodo);

        if (bf > 1 && esMenor(t, nodo->left->key)) return rotateRight(nodo);
        if (bf < -1 && !esMenor(t, nodo->right->key)) return rotateLeft(nodo);
        if (bf > 1 && !esMenor(t, nodo->left->key)) { nodo->left = rotateLeft(nodo->left); return rotateRight(nodo); }
        if (bf < -1 && esMenor(t, nodo->right->key)) { nodo->right = rotateRight(nodo->right); return rotateLeft(nodo); }
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
                delete nodo; return hijo;
            } else {
                AVLNode<Transaccion*>* sucesor = minimoNodo(nodo->right);
                nodo->key = sucesor->key;
                nodo->right = eliminarNodo(nodo->right, sucesor->key);
            }
        } else if (esMenor(objetivo, nodo->key)) nodo->left = eliminarNodo(nodo->left, objetivo);
        else nodo->right = eliminarNodo(nodo->right, objetivo);

        if (!nodo) return nullptr;

        nodo->height = 1 + max(height(nodo->left), height(nodo->right));
        int bf = balanceFactor(nodo);

        if (bf > 1 && balanceFactor(nodo->left) >= 0) return rotateRight(nodo);
        if (bf > 1 && balanceFactor(nodo->left) < 0) { nodo->left = rotateLeft(nodo->left); return rotateRight(nodo); }
        if (bf < -1 && balanceFactor(nodo->right) <= 0) return rotateLeft(nodo);
        if (bf < -1 && balanceFactor(nodo->right) > 0) { nodo->right = rotateRight(nodo->right); return rotateLeft(nodo); }
        return nodo;
    }

    void inorden(AVLNode<Transaccion*>* nodo, int& contador, int limite) {
        if (!nodo || contador >= limite) return;
        inorden(nodo->left, contador, limite);
        if (contador < limite) { nodo->key->mostrar(); contador++; }
        inorden(nodo->right, contador, limite);
    }

    void inordenRango(AVLNode<Transaccion*>* nodo, const string& fechaInicio, const string& fechaFin, int& contador) {
        if (!nodo) return;
        string fh = fechaHoraCompleta(*nodo->key);
        if (fh > fechaInicio) inordenRango(nodo->left, fechaInicio, fechaFin, contador);
        if (fh >= fechaInicio && fh <= fechaFin) { nodo->key->mostrar(); contador++; }
        if (fh < fechaFin) inordenRango(nodo->right, fechaInicio, fechaFin, contador);
    }

    void liberar(AVLNode<Transaccion*>* nodo) {
        if (!nodo) return;
        liberar(nodo->left); liberar(nodo->right); delete nodo;
    }

public:
    AVLTree() : raiz(nullptr) {}
    ~AVLTree() { liberar(raiz); }

    void insertar(Transaccion* t) { raiz = insertarNodo(raiz, t); }
    bool eliminar(Transaccion* objetivo) {
        if (!objetivo) return false;
        raiz = eliminarNodo(raiz, objetivo); return true;
    }
    void mostrarInorden(int limite = 50) {
        int contador = 0; inorden(raiz, contador, limite);
        cout << "Se mostraron " << contador << " transacciones cronologicamente.\n";
    }
    int consultarRango(const string& fechaInicio, const string& fechaFin) {
        int contador = 0; inordenRango(raiz, fechaInicio, fechaFin + " 23:59:59", contador);
        cout << "Transacciones encontradas: " << contador << "\n"; return contador;
    }
};


void cargarDesdeCSV(const string& filename, hashTable& hash, AVLTree& avl) {
    FILE* archivo = fopen(filename.c_str(), "r");
    if (!archivo) {
        cout << "Error: No se pudo abrir el archivo " << filename << "\n";
        return;
    }

    char linea[1024];
    int insertados = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        if (linea[0] == '\n' || strstr(linea, "idTransaccion") != nullptr) continue;

        Transaccion* t = new Transaccion();
        char* token = strtok(linea, ",\n\r");
        if (token) t->idTransaccion = token;

        token = strtok(nullptr, ",\n\r"); if (token) t->cuentaOrigen = token;
        token = strtok(nullptr, ",\n\r"); if (token) t->cliente = token;
        token = strtok(nullptr, ",\n\r"); if (token) t->tipo = token;
        token = strtok(nullptr, ",\n\r"); if (token) t->monto = atof(token);
        token = strtok(nullptr, ",\n\r"); if (token) t->fecha = token;
        token = strtok(nullptr, ",\n\r"); if (token) t->hora = token;
        token = strtok(nullptr, ",\n\r"); if (token) t->estado = token;

        if (hash.insert(t)) {
            avl.insertar(t);
            insertados++;
        } else {
            delete t;
        }
    }
    fclose(archivo);
    cout << "Carga masiva completada. Registros insertados: " << insertados << "\n";
}

int main() {
    hashTable tablaHash(20000);
    AVLTree arbolAVL;
    int opcion;

    do {
        cout << "\n=== SISTEMA DE TRANSACCIONES BANCARIAS ===\n";
        cout << "1. Carga masiva desde archivo\n";
        cout << "2. Registrar nueva transaccion\n";
        cout << "3. Buscar transaccion por ID\n";
        cout << "4. Consulta cronologica (Mostrar 50)\n";
        cout << "5. Consulta por rango de fechas\n";
        cout << "6. Actualizar estado de transaccion\n";
        cout << "7. Eliminar transaccion\n";
        cout << "8. Ver estadisticas generales\n";
        cout << "9. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string archivo;
                cout << "Ingrese nombre del archivo (ej. transacciones_masivo.csv): ";
                cin >> archivo;
                cargarDesdeCSV(archivo, tablaHash, arbolAVL);
                break;
            }
            case 2: {
                Transaccion* t = new Transaccion();
                cout << "ID: "; cin >> t->idTransaccion;

                if (tablaHash.search(t->idTransaccion) != nullptr) {
                    cout << "Error: El ID ya existe.\n";
                    delete t;
                    break;
                }

                cout << "Cuenta Origen: "; cin >> t->cuentaOrigen;
                cout << "Cliente (sin espacios): "; cin >> t->cliente;
                cout << "Tipo: "; cin >> t->tipo;
                cout << "Monto: "; cin >> t->monto;
                cout << "Fecha (YYYY-MM-DD): "; cin >> t->fecha;
                cout << "Hora (HH:MM:SS): "; cin >> t->hora;
                cout << "Estado: "; cin >> t->estado;

                tablaHash.insert(t);
                arbolAVL.insertar(t);
                cout << "Transaccion registrada exitosamente.\n";
                break;
            }
            case 3: {
                string id;
                cout << "Ingrese ID a buscar: "; cin >> id;
                Transaccion* t = tablaHash.search(id);
                if (t) t->mostrar();
                else cout << "Transaccion no encontrada.\n";
                break;
            }
            case 4:
                arbolAVL.mostrarInorden(50);
                break;
            case 5: {
                string f1, f2;
                cout << "Fecha inicial (YYYY-MM-DD): "; cin >> f1;
                cout << "Fecha final (YYYY-MM-DD): "; cin >> f2;
                arbolAVL.consultarRango(f1, f2);
                break;
            }
            case 6: {
                string id, nuevoEstado;
                cout << "Ingrese ID de la transaccion: "; cin >> id;
                Transaccion* t = tablaHash.search(id);
                if (t) {
                    cout << "Estado actual: " << t->estado << "\n";
                    cout << "Ingrese nuevo estado (Ej. Aprobada, Rechazada): ";
                    cin >> nuevoEstado;
                    t->estado = nuevoEstado;
                    cout << "Estado actualizado.\n";
                } else {
                    cout << "Transaccion no encontrada.\n";
                }
                break;
            }
            case 7: {
                string id;
                cout << "Ingrese ID de la transaccion a eliminar: "; cin >> id;
                Transaccion* t = tablaHash.search(id);
                if (t) {
                    arbolAVL.eliminar(t);
                    tablaHash.remove(id);
                    delete t;
                    cout << "Transaccion eliminada correctamente.\n";
                } else {
                    cout << "Transaccion no encontrada.\n";
                }
                break;
            }
            case 8:
                tablaHash.calcularEstadisticas();
                break;
            case 9:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
    } while (opcion != 9);

    return 0;
}
