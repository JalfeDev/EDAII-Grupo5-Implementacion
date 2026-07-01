#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Transaccion {
private:
    string id;
    string cuentaOrigen;
    string cliente;
    string tipo;
    double monto;
    string fecha;
    string hora;
    string estado;

public:
    Transaccion(string i, string co, string c, string ti, double m, string f, string h, string e) {
        id = i;
        cuentaOrigen = co;
        cliente = c;
        tipo = ti;
        monto = m;
        fecha = f;
        hora = h;
        estado = e;
    }

    string getId() { return id; }
    string getCuentaOrigen() { return cuentaOrigen; }
    string getCliente() { return cliente; }
    string getTipo() { return tipo; }
    double getMonto() { return monto; }
    string getFecha() { return fecha; }
    string getHora() { return hora; }
    string getEstado() { return estado; }

    void setEstado(string e) {
        estado = e;
    }

    long long getClaveFechaHora() {
        string clave = "";

        for(int i = 0; i < fecha.size(); i++) {
            if(fecha[i] != '-') {
                clave = clave + fecha[i];
            }
        }

        for(int i = 0; i < hora.size(); i++) {
            if(hora[i] != ':') {
                clave = clave + hora[i];
            }
        }

        string numeroId = "";

        for(int i = 0; i < id.size(); i++) {
            if(id[i] >= '0' && id[i] <= '9') {
                numeroId = numeroId + id[i];
            }
        }

        string ultimos = "";
        int contador = 0;

        for(int i = numeroId.size() - 1; i >= 0 && contador < 5; i--) {
            ultimos = numeroId[i] + ultimos;
            contador++;
        }

        while(ultimos.size() < 5) {
            ultimos = "0" + ultimos;
        }

        clave = clave + ultimos;

        long long num = 0;

        for(int i = 0; i < clave.size(); i++) {
            num = num * 10 + (clave[i] - '0');
        }

        return num;
    }

    void mostrar() {
        cout << id << " | "
             << cuentaOrigen << " | "
             << cliente << " | "
             << tipo << " | "
             << monto << " | "
             << fecha << " | "
             << hora << " | "
             << estado << endl;
    }
};

class HashNode {
private:
    string key;
    Transaccion* data;
    HashNode* next;

public:
    HashNode(string k, Transaccion* t) {
        key = k;
        data = t;
        next = nullptr;
    }

    string getKey() { return key; }
    Transaccion* getData() { return data; }
    HashNode* getNext() { return next; }

    void setNext(HashNode* tmp) {
        next = tmp;
    }
};

class LinkedList {
private:
    HashNode* head;

public:
    LinkedList() {
        head = nullptr;
    }

    Transaccion* buscar(string key) {
        HashNode* tmp = head;

        while(tmp != nullptr) {
            if(tmp->getKey() == key) {
                return tmp->getData();
            }

            tmp = tmp->getNext();
        }

        return nullptr;
    }

    bool insertar(string key, Transaccion* t) {
        if(buscar(key) != nullptr) {
            return false;
        }

        HashNode* nodito = new HashNode(key, t);

        if(head == nullptr) {
            head = nodito;
            return true;
        }

        HashNode* tmp = head;

        while(tmp->getNext() != nullptr) {
            tmp = tmp->getNext();
        }

        tmp->setNext(nodito);

        return true;
    }

    bool eliminar(string key) {
        if(head == nullptr) {
            return false;
        }

        if(head->getKey() == key) {
            HashNode* tmp = head;
            head = head->getNext();
            delete tmp;
            return true;
        }

        HashNode* anterior = head;
        HashNode* actual = head->getNext();

        while(actual != nullptr) {
            if(actual->getKey() == key) {
                anterior->setNext(actual->getNext());
                delete actual;
                return true;
            }

            anterior = actual;
            actual = actual->getNext();
        }

        return false;
    }
};

class HashTable {
private:
    LinkedList* table;
    int size;
    int cantidad;

public:
    HashTable(int s) {
        size = s;
        cantidad = 0;
        table = new LinkedList[size];
    }

    ~HashTable() {
        delete[] table;
    }

    int hashFunction(string key) {
        int suma = 0;

        for(int i = 0; i < key.size(); i++) {
            suma = suma + key[i];
        }

        return suma % size;
    }

    bool insertar(Transaccion* t) {
        int pos = hashFunction(t->getId());

        if(table[pos].insertar(t->getId(), t)) {
            cantidad++;
            return true;
        }

        return false;
    }

    Transaccion* buscar(string key) {
        int pos = hashFunction(key);
        return table[pos].buscar(key);
    }

    bool eliminar(string key) {
        int pos = hashFunction(key);

        if(table[pos].eliminar(key)) {
            cantidad--;
            return true;
        }

        return false;
    }

    int getCantidad() {
        return cantidad;
    }
};

class AVLNode {
private:
    long long key;
    Transaccion* data;
    int h;
    AVLNode* left;
    AVLNode* right;

public:
    AVLNode(long long k, Transaccion* t) {
        key = k;
        data = t;
        h = 0;
        left = nullptr;
        right = nullptr;
    }

    long long getKey() { return key; }
    void setKey(long long k) { key = k; }

    Transaccion* getData() { return data; }
    void setData(Transaccion* t) { data = t; }

    int getH() { return h; }
    void setH(int he) { h = he; }

    AVLNode* getLeft() { return left; }
    void setLeft(AVLNode* tmp) { left = tmp; }

    AVLNode* getRight() { return right; }
    void setRight(AVLNode* tmp) { right = tmp; }
};

class AVL {
private:
    AVLNode* root;
    int cantidad;

public:
    AVL() {
        root = nullptr;
        cantidad = 0;
    }

    int altura(AVLNode* n) {
        if(n == nullptr)
            return -1;

        return n->getH();
    }

    int maximo(int a, int b) {
        if(a > b)
            return a;
        else
            return b;
    }

    int getFE(AVLNode* n) {
        if(n == nullptr)
            return 0;

        return altura(n->getLeft()) - altura(n->getRight());
    }

    void updateH(AVLNode* n) {
        if(n != nullptr)
            n->setH(1 + maximo(altura(n->getLeft()), altura(n->getRight())));
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->getLeft();
        AVLNode* T2 = x->getRight();

        x->setRight(y);
        y->setLeft(T2);

        updateH(y);
        updateH(x);

        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->getRight();
        AVLNode* T2 = y->getLeft();

        y->setLeft(x);
        x->setRight(T2);

        updateH(x);
        updateH(y);

        return y;
    }

    void insert(Transaccion* t) {
        root = insertR(root, t->getClaveFechaHora(), t);
        cantidad++;
    }

    AVLNode* insertR(AVLNode* node, long long x, Transaccion* t) {
        if(node == nullptr)
            return new AVLNode(x, t);

        if(x < node->getKey())
            node->setLeft(insertR(node->getLeft(), x, t));
        else
            node->setRight(insertR(node->getRight(), x, t));

        updateH(node);

        int bf = getFE(node);

        if(bf > 1 && x < node->getLeft()->getKey())
            return rotateRight(node);

        if(bf < -1 && x > node->getRight()->getKey())
            return rotateLeft(node);

        if(bf < -1 && x < node->getRight()->getKey()) {
            node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }

        if(bf > 1 && x > node->getLeft()->getKey()) {
            node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }

        return node;
    }

    AVLNode* nodoMinimo(AVLNode* node) {
        AVLNode* tmp = node;

        while(tmp->getLeft() != nullptr) {
            tmp = tmp->getLeft();
        }

        return tmp;
    }

    bool existeClave(long long key) {
        return existeClaveR(root, key);
    }

    bool existeClaveR(AVLNode* node, long long key) {
        if(node == nullptr)
            return false;

        if(key == node->getKey())
            return true;

        if(key < node->getKey())
            return existeClaveR(node->getLeft(), key);
        else
            return existeClaveR(node->getRight(), key);
    }

    bool eliminar(long long key) {
        if(existeClave(key) == false) {
            return false;
        }

        root = eliminarR(root, key);
        cantidad--;
        return true;
    }

    AVLNode* eliminarR(AVLNode* node, long long key) {
        if(node == nullptr)
            return node;

        if(key < node->getKey()) {
            node->setLeft(eliminarR(node->getLeft(), key));
        }
        else if(key > node->getKey()) {
            node->setRight(eliminarR(node->getRight(), key));
        }
        else {
            if(node->getLeft() == nullptr || node->getRight() == nullptr) {
                AVLNode* tmp;

                if(node->getLeft() != nullptr)
                    tmp = node->getLeft();
                else
                    tmp = node->getRight();

                delete node;
                return tmp;
            }
            else {
                AVLNode* tmp = nodoMinimo(node->getRight());

                node->setKey(tmp->getKey());
                node->setData(tmp->getData());

                node->setRight(eliminarR(node->getRight(), tmp->getKey()));
            }
        }

        updateH(node);

        int bf = getFE(node);

        if(bf > 1 && getFE(node->getLeft()) >= 0)
            return rotateRight(node);

        if(bf > 1 && getFE(node->getLeft()) < 0) {
            node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }

        if(bf < -1 && getFE(node->getRight()) <= 0)
            return rotateLeft(node);

        if(bf < -1 && getFE(node->getRight()) > 0) {
            node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }

        return node;
    }

    void inOrdenLimitado(int limite) {
        int contador = 0;
        inOrdenLimitadoR(root, contador, limite);
    }

    void inOrdenLimitadoR(AVLNode* node, int& contador, int limite) {
        if(node == nullptr || contador >= limite)
            return;

        inOrdenLimitadoR(node->getLeft(), contador, limite);

        if(contador < limite) {
            node->getData()->mostrar();
            contador++;
        }

        inOrdenLimitadoR(node->getRight(), contador, limite);
    }

    void buscarRango(long long inicio, long long fin) {
        int cantidadEncontrada = 0;
        int muestra = 0;

        buscarRangoR(root, inicio, fin, cantidadEncontrada, muestra);

        cout << "Cantidad encontrada: " << cantidadEncontrada << endl;
    }

    void buscarRangoR(AVLNode* node, long long inicio, long long fin, int& cantidadEncontrada, int& muestra) {
        if(node == nullptr)
            return;

        if(node->getKey() > inicio)
            buscarRangoR(node->getLeft(), inicio, fin, cantidadEncontrada, muestra);

        if(node->getKey() >= inicio && node->getKey() <= fin) {
            cantidadEncontrada++;

            if(muestra < 20) {
                node->getData()->mostrar();
                muestra++;
            }
        }

        if(node->getKey() < fin)
            buscarRangoR(node->getRight(), inicio, fin, cantidadEncontrada, muestra);
    }

    void estadisticas(double& montoTotal, Transaccion*& mayor, Transaccion*& menor,
                      int tipos[], int estados[]) {
        recorrerEstadisticas(root, montoTotal, mayor, menor, tipos, estados);
    }

    void recorrerEstadisticas(AVLNode* node, double& montoTotal, Transaccion*& mayor, Transaccion*& menor,
                              int tipos[], int estados[]) {
        if(node == nullptr)
            return;

        recorrerEstadisticas(node->getLeft(), montoTotal, mayor, menor, tipos, estados);

        Transaccion* t = node->getData();

        montoTotal = montoTotal + t->getMonto();

        if(mayor == nullptr || t->getMonto() > mayor->getMonto())
            mayor = t;

        if(menor == nullptr || t->getMonto() < menor->getMonto())
            menor = t;

        if(t->getTipo() == "Transferencia")
            tipos[0]++;
        else if(t->getTipo() == "Retiro")
            tipos[1]++;
        else if(t->getTipo() == "Deposito")
            tipos[2]++;
        else if(t->getTipo() == "PagoServicio")
            tipos[3]++;
        else
            tipos[4]++;

        if(t->getEstado() == "Aprobada")
            estados[0]++;
        else if(t->getEstado() == "Rechazada")
            estados[1]++;
        else if(t->getEstado() == "Pendiente")
            estados[2]++;
        else if(t->getEstado() == "Observada")
            estados[3]++;
        else if(t->getEstado() == "Anulada")
            estados[4]++;

        recorrerEstadisticas(node->getRight(), montoTotal, mayor, menor, tipos, estados);
    }

    int getCantidad() {
        return cantidad;
    }
};

class Sistema {
private:
    HashTable* tabla;
    AVL* arbol;

public:
    Sistema() {
        tabla = new HashTable(20011);
        arbol = new AVL();
    }

    ~Sistema() {
        delete tabla;
        delete arbol;
    }

    string limpiar(string texto) {
        while(texto.size() > 0 && texto[0] == ' ') {
            string aux = "";

            for(int i = 1; i < texto.size(); i++) {
                aux = aux + texto[i];
            }

            texto = aux;
        }

        while(texto.size() > 0 && texto[texto.size() - 1] == ' ') {
            string aux = "";

            for(int i = 0; i < texto.size() - 1; i++) {
                aux = aux + texto[i];
            }

            texto = aux;
        }

        return texto;
    }

    string obtenerCampo(string linea, int& pos) {
        string campo = "";

        while(pos < linea.size() && linea[pos] != ',') {
            campo = campo + linea[pos];
            pos++;
        }

        pos++;

        return limpiar(campo);
    }

    double convertirDouble(string texto) {
        double numero = 0;
        double decimal = 0.1;
        bool punto = false;

        for(int i = 0; i < texto.size(); i++) {
            if(texto[i] == '.') {
                punto = true;
            }
            else if(texto[i] >= '0' && texto[i] <= '9') {
                if(punto == false) {
                    numero = numero * 10 + (texto[i] - '0');
                }
                else {
                    numero = numero + (texto[i] - '0') * decimal;
                    decimal = decimal / 10;
                }
            }
        }

        return numero;
    }

    bool insertarTransaccion(Transaccion* t) {
        if(tabla->buscar(t->getId()) != nullptr) {
            cout << "El ID ya existe, no se inserta" << endl;
            return false;
        }

        tabla->insertar(t);
        arbol->insert(t);

        return true;
    }

    void cargarArchivo(string nombreArchivo) {
        string rutaCompleta = "C:\\Users\\" + "User" + "\\Desktop\\EDA2\\Proyecto Integrador\\" + nombreArchivo;
        ifstream archivo(rutaCompleta.c_str());
        if(!archivo.is_open()) {
            cout << "No se pudo abrir el archivo" << endl;
            return;
        }

        string linea;
        int contador = 0;

        while(getline(archivo, linea)) {
            if(linea == "")
                continue;

            if(linea.find("idTransaccion") != string::npos)
                continue;

            int pos = 0;

            string id = obtenerCampo(linea, pos);
            string cuentaOrigen = obtenerCampo(linea, pos);
            string cliente = obtenerCampo(linea, pos);
            string tipo = obtenerCampo(linea, pos);
            string montoTexto = obtenerCampo(linea, pos);
            string fecha = obtenerCampo(linea, pos);
            string hora = obtenerCampo(linea, pos);
            string estado = obtenerCampo(linea, pos);

            double monto = convertirDouble(montoTexto);

            Transaccion* t = new Transaccion(id, cuentaOrigen, cliente, tipo, monto, fecha, hora, estado);

            if(insertarTransaccion(t)) {
                contador++;
            }
            else {
                delete t;
            }
        }

        archivo.close();

        cout << "Transacciones cargadas: " << contador << endl;
        cout << "Cantidad en hash: " << tabla->getCantidad() << endl;
        cout << "Cantidad en AVL: " << arbol->getCantidad() << endl;
    }

    void registrarManual() {
        string id, cuenta, cliente, tipo, fecha, hora, estado;
        double monto;

        cout << "ID: ";
        cin >> id;

        cout << "Cuenta origen: ";
        cin >> cuenta;

        cout << "Cliente sin espacios: ";
        cin >> cliente;

        cout << "Tipo: ";
        cin >> tipo;

        cout << "Monto: ";
        cin >> monto;

        cout << "Fecha AAAA-MM-DD: ";
        cin >> fecha;

        cout << "Hora HH:MM:SS: ";
        cin >> hora;

        cout << "Estado: ";
        cin >> estado;

        Transaccion* t = new Transaccion(id, cuenta, cliente, tipo, monto, fecha, hora, estado);

        if(insertarTransaccion(t)) {
            cout << "Transaccion insertada correctamente" << endl;
        }
        else {
            delete t;
        }
    }

    void buscarPorId(string id) {
        Transaccion* t = tabla->buscar(id);

        if(t == nullptr) {
            cout << "Transaccion no encontrada" << endl;
        }
        else {
            t->mostrar();
        }
    }

    void mostrarOrdenado() {
        arbol->inOrdenLimitado(50);
    }

    long long claveDesdeFechaHora(string fecha, string hora, string ultimos) {
        Transaccion aux("TX-" + ultimos, "", "", "", 0, fecha, hora, "");
        return aux.getClaveFechaHora();
    }

    void consultarRango(string fechaInicio, string fechaFin) {
        long long inicio = claveDesdeFechaHora(fechaInicio, "00:00:00", "00000");
        long long fin = claveDesdeFechaHora(fechaFin, "23:59:59", "99999");

        arbol->buscarRango(inicio, fin);
    }

    void actualizarEstado(string id, string nuevoEstado) {
        Transaccion* t = tabla->buscar(id);

        if(t == nullptr) {
            cout << "Transaccion no encontrada" << endl;
            return;
        }

        t->setEstado(nuevoEstado);

        cout << "Estado actualizado correctamente" << endl;
    }

    void eliminarTransaccion(string id) {
        Transaccion* t = tabla->buscar(id);

        if(t == nullptr) {
            cout << "Transaccion no encontrada" << endl;
            return;
        }

        long long clave = t->getClaveFechaHora();

        bool eliminadoArbol = arbol->eliminar(clave);
        bool eliminadoHash = tabla->eliminar(id);

        if(eliminadoArbol && eliminadoHash) {
            delete t;
            cout << "Transaccion eliminada correctamente" << endl;
        }
        else {
            cout << "Error al eliminar la transaccion" << endl;
        }
    }

    void mostrarEstadisticas() {
        double montoTotal = 0;
        Transaccion* mayor = nullptr;
        Transaccion* menor = nullptr;

        int tipos[5];
        int estados[5];

        for(int i = 0; i < 5; i++) {
            tipos[i] = 0;
            estados[i] = 0;
        }

        arbol->estadisticas(montoTotal, mayor, menor, tipos, estados);

        int total = arbol->getCantidad();

        cout << "\nESTADISTICAS GENERALES" << endl;
        cout << "Cantidad total: " << total << endl;
        cout << "Monto total: " << montoTotal << endl;

        if(total > 0)
            cout << "Monto promedio: " << montoTotal / total << endl;

        cout << "\nCantidad por tipo:" << endl;
        cout << "Transferencia: " << tipos[0] << endl;
        cout << "Retiro: " << tipos[1] << endl;
        cout << "Deposito: " << tipos[2] << endl;
        cout << "PagoServicio: " << tipos[3] << endl;
        cout << "Otros: " << tipos[4] << endl;

        cout << "\nCantidad por estado:" << endl;
        cout << "Aprobada: " << estados[0] << endl;
        cout << "Rechazada: " << estados[1] << endl;
        cout << "Pendiente: " << estados[2] << endl;
        cout << "Observada: " << estados[3] << endl;
        cout << "Anulada: " << estados[4] << endl;

        cout << "\nTransaccion de mayor monto:" << endl;
        if(mayor != nullptr)
            mayor->mostrar();

        cout << "\nTransaccion de menor monto:" << endl;
        if(menor != nullptr)
            menor->mostrar();
    }
};

int main() {
    Sistema* sistema = new Sistema();

    int opcion = 0;

    while(opcion != 9) {
        cout << "\n SISTEMA DE TRANSACCIONES BANCARIAS" << endl;
        cout << "1. Cargar archivo CSV" << endl;
        cout << "2. Registrar nueva transaccion" << endl;
        cout << "3. Buscar por ID" << endl;
        cout << "4. Mostrar ordenado por fecha y hora" << endl;
        cout << "5. Consultar por rango de fechas" << endl;
        cout << "6. Actualizar estado" << endl;
        cout << "7. Eliminar transaccion" << endl;
        cout << "8. Estadisticas generales" << endl;
        cout << "9. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        if(opcion == 1) {
            string archivo;

            cout << "Nombre del archivo CSV: ";
            cin >> archivo;

            auto inicio = high_resolution_clock::now();

            sistema->cargarArchivo(archivo);

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo carga masiva: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 2) {
            auto inicio = high_resolution_clock::now();

            sistema->registrarManual();

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo insercion individual: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 3) {
            string id;

            cout << "ID a buscar: ";
            cin >> id;

            auto inicio = high_resolution_clock::now();

            sistema->buscarPorId(id);

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo busqueda: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 4) {
            auto inicio = high_resolution_clock::now();

            sistema->mostrarOrdenado();

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo consulta ordenada: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 5) {
            string fechaInicio;
            string fechaFin;

            cout << "Fecha inicial AAAA-MM-DD: ";
            cin >> fechaInicio;

            cout << "Fecha final AAAA-MM-DD: ";
            cin >> fechaFin;

            auto inicio = high_resolution_clock::now();

            sistema->consultarRango(fechaInicio, fechaFin);

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo consulta por rango: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 6) {
            string id;
            string estado;

            cout << "ID: ";
            cin >> id;

            cout << "Nuevo estado: ";
            cin >> estado;

            auto inicio = high_resolution_clock::now();

            sistema->actualizarEstado(id, estado);

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo actualizacion: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 7) {
            string id;

            cout << "ID a eliminar: ";
            cin >> id;

            auto inicio = high_resolution_clock::now();

            sistema->eliminarTransaccion(id);

            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<microseconds>(fin - inicio);

            cout << "Tiempo eliminacion: " << duracion.count() << " microsegundos" << endl;
        }
        else if(opcion == 8) {
            sistema->mostrarEstadisticas();
        }
    }

    delete sistema;

    return 0;
}
