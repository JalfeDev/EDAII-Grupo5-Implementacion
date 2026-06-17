#include "hashtable.h"

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
