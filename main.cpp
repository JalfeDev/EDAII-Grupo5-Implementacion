#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "avl.h"
#include "hashtable.h"
#include "redblack.h"
#include "transaccion.h
using namespace std;

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