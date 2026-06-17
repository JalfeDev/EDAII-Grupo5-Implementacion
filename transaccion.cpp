#include "transaccion.h"

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