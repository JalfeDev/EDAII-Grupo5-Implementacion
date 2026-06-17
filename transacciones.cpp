#include <iostream>
#include <fstream>
#include <sstream>
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


struct HashNode {
  Transaccion* tx;
  HashNode* next;
  HashNode(Transaccion* t) : tx(t), next(nullptr) {}
};

class HashTable {
private:
  int size;
  int count;
  HashNode** table;

   
  int hashFunction(const string& id) const {
    int suma = 0;
    for (int i = 0; i < (int)id.size(); i++) {
      suma += (int)id[i];
    }
    return suma % size;
  }

public:
  HashTable(int s = 20000) : size(s), count(0) {
    table = new HashNode*[size];
    for (int i = 0; i < size; i++) table[i] = nullptr;
  }

  ~HashTable() {
    for (int i = 0; i < size; i++) {
      HashNode* entry = table[i];
      while (entry != nullptr) {
        HashNode* prev = entry;
        entry = entry->next;
        delete prev;
      }
    }
    delete[] table;
  }

  bool insertar(Transaccion* t) {
    int hashVal = hashFunction(t->idTransaccion);
    HashNode* entry = table[hashVal];
     
    while (entry != nullptr) {
      if (entry->tx->idTransaccion == t->idTransaccion) return false; // Ya existe
      entry = entry->next;
    }
     
    HashNode* newNode = new HashNode(t);
    newNode->next = table[hashVal];
    table[hashVal] = newNode;
    count++;
    return true;
  }

  Transaccion* buscar(const string& id) {
    int hashVal = hashFunction(id);
    HashNode* entry = table[hashVal];
    while (entry != nullptr) {
      if (entry->tx->idTransaccion == id) return entry->tx;
      entry = entry->next;
    }
    return nullptr;
  }

  bool eliminar(const string& id) {
    int hashVal = hashFunction(id);
    HashNode* entry = table[hashVal];
    HashNode* prev = nullptr;

    while (entry != nullptr && entry->tx->idTransaccion != id) {
      prev = entry;
      entry = entry->next;
    }

    if (entry == nullptr) return false; 

    if (prev == nullptr) {
      table[hashVal] = entry->next;
    } else {
      prev->next = entry->next;
    }
     
    delete entry;
    count--;
    return true;
  }

  int getCount() const { return count; }
   
  void calcularEstadisticas() {
    if (count == 0) {
      cout << "No hay transacciones registradas.\n";
      return;
    }
    double montoTotal = 0;
    double maxMonto = -1, minMonto = 999999999;
    string idMax, idMin;

    for (int i = 0; i < size; i++) {
      HashNode* entry = table[i];
      while (entry != nullptr) {
        montoTotal += entry->tx->monto;
        if (entry->tx->monto > maxMonto) {
          maxMonto = entry->tx->monto;
          idMax = entry->tx->idTransaccion;
        }
        if (entry->tx->monto < minMonto) {
          minMonto = entry->tx->monto;
          idMin = entry->tx->idTransaccion;
        }
        entry = entry->next;
      }
    }
    cout << "--- ESTADISTICAS ---\n";
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


void cargarDesdeCSV(const string& filename, HashTable& hash, AVLTree& avl) {
  ifstream archivo(filename);
  if (!archivo.is_open()) {
    cout << "Error: No se pudo abrir el archivo " << filename << "\n";
    return;
  }

  string linea;
  int insertados = 0;
   
  while (getline(archivo, linea)) {
    if (linea.empty() || linea.find("idTransaccion") != string::npos) continue;

    stringstream ss(linea);
    string item;
    Transaccion* t = new Transaccion();

    getline(ss, t->idTransaccion, ',');
    getline(ss, t->cuentaOrigen, ',');
    getline(ss, t->cliente, ',');
    getline(ss, t->tipo, ',');
    getline(ss, item, ','); t->monto = stod(item);
    getline(ss, t->fecha, ',');
    getline(ss, t->hora, ',');
    getline(ss, t->estado, ',');

    if (hash.insertar(t)) {
      avl.insertar(t);
      insertados++;
    } else {
      delete t;
    }
  }
  archivo.close();
  cout << "Carga masiva completada. Registros insertados: " << insertados << "\n";
}

int main() {
  HashTable tablaHash(20000);
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
         
        if (tablaHash.buscar(t->idTransaccion) != nullptr) {
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

        tablaHash.insertar(t);
        arbolAVL.insertar(t);
        cout << "Transaccion registrada exitosamente.\n";
        break;
      }
      case 3: {
        string id;
        cout << "Ingrese ID a buscar: ";
        cin >> id;
        Transaccion* t = tablaHash.buscar(id);
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
        Transaccion* t = tablaHash.buscar(id);
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
        Transaccion* t = tablaHash.buscar(id);
        if (t) {
          arbolAVL.eliminar(t);
          tablaHash.eliminar(id);
          delete t;
          cout << "Transaccion eliminada correctamente de ambas estructuras.\n";
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