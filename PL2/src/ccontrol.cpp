#include "ccontrol.h"

// --- Funciones Auxiliares ---
int generarIdLibreria() {
    return rand() % 900 + 100; // 100-999
}

void generarIdPedido(char* buffer) {
    sprintf(buffer, "P%05d", rand() % 100000);
}

void generarCodLibro(char* buffer) {
    char letra = 'A' + rand() % 26;
    sprintf(buffer, "%03d%c%02d", rand() % 1000, letra, rand() % 100);
}

void generarMateria(char* buffer) {
    const char* materias[] = {"Matematicas", "Fisica", "Tecnologia", "Musica", "Historia", "Lengua"};
    strcpy(buffer, materias[rand() % 6]);
}

void generarFecha(char* buffer) {
    int dia = rand() % 28 + 1;
    int mes = rand() % 12 + 1;
    sprintf(buffer, "%02d-%02d-2025", dia, mes);
}

void generarLocalidad(char* buffer) {
    const char* localidades[] = { "Mostoles", "Alcala", "Leganes", "Fuenlabrada", "Getafe",
                                  "Alcorcon", "Torrejon", "Parla", "Alcobendas", "Coslada",
                                  "Pozuelo", "Rivas", "Valdemoro", "Majadahonda", "Aranjuez",
                                  "Arganda", "Boadilla", "Pinto", "Colmenar", "Tres Cantos" };
    strcpy(buffer, localidades[rand() % 20]);
}

Pedido generarPedidoAleatorio() {
    Pedido p;
    p.id_libreria = 0; // Se asignará luego
    generarIdPedido(p.id_pedido);
    generarCodLibro(p.cod_libro);
    generarMateria(p.materia);
    p.unidades = rand() % 20 + 1;
    generarFecha(p.fecha_envio);
    return p;
}

Libreria generarLibreriaAleatoria() {
    Libreria l;
    l.id_libreria = generarIdLibreria();
    generarLocalidad(l.localidad);
    // La lista se inicializa sola por el constructor de ListaPedidos
    return l;
}

// --- Implementación ListaPedidos ---

ListaPedidos::ListaPedidos() {
    cabecera = NULL;
    cola = NULL;
}

ListaPedidos::~ListaPedidos() {
    NodoPedido* aux = cabecera;
    while (aux != NULL) {
        NodoPedido* borrar = aux;
        aux = aux->sig;
        delete borrar;
    }
}

void ListaPedidos::insertar(Pedido p) {
    NodoPedido* nuevo = new NodoPedido;
    nuevo->datos = p;
    nuevo->sig = NULL;

    if (cabecera == NULL) {
        cabecera = nuevo;
        cola = nuevo;
    } else {
        cola->sig = nuevo;
        cola = nuevo;
    }
}

void ListaPedidos::borrar(const char* id_pedido) {
    if (cabecera == NULL) return;

    // Caso: borrar cabecera
    if (strcmp(cabecera->datos.id_pedido, id_pedido) == 0) {
        NodoPedido* borrar = cabecera;
        cabecera = cabecera->sig;
        if (cabecera == NULL) cola = NULL;
        delete borrar;
        return;
    }

    NodoPedido* actual = cabecera;
    while (actual->sig != NULL && strcmp(actual->sig->datos.id_pedido, id_pedido) != 0) {
        actual = actual->sig;
    }

    if (actual->sig != NULL) {
        NodoPedido* borrar = actual->sig;
        actual->sig = borrar->sig;
        if (actual->sig == NULL) cola = actual;
        delete borrar;
    }
}

NodoPedido* ListaPedidos::buscar(const char* id_pedido) {
    NodoPedido* actual = cabecera;
    while (actual != NULL) {
        if (strcmp(actual->datos.id_pedido, id_pedido) == 0) {
            return actual;
        }
        actual = actual->sig;
    }
    return NULL;
}

int ListaPedidos::getNumeroPedidos() {
    int count = 0;
    NodoPedido* aux = cabecera;
    while(aux != NULL) {
        count++;
        aux = aux->sig;
    }
    return count;
}

void ListaPedidos::mostrar() {
    NodoPedido* actual = cabecera;
    if (actual == NULL) {
        cout << "   (Sin pedidos)" << endl;
        return;
    }

    // Cabecera de la tabla local
    cout << "   --------------------------------------------------------------" << endl;
    cout << "   |" << left << setw(10) << "ID Pedido"
         << "|" << setw(10) << "Cod Libro"
         << "|" << setw(15) << "Materia"
         << "|" << setw(9)  << "Unidades"
         << "|" << setw(12) << "Fecha" << "|" << endl;
    cout << "   --------------------------------------------------------------" << endl;

    while (actual != NULL) {
        cout << "   |" << left << setw(10) << actual->datos.id_pedido
             << "|" << setw(10) << actual->datos.cod_libro
             << "|" << setw(15) << actual->datos.materia
             << "|" << right << setw(8) << actual->datos.unidades << " "
             << "|" << left  << setw(12) << actual->datos.fecha_envio << "|" << endl;
        actual = actual->sig;
    }
    cout << "   --------------------------------------------------------------" << endl;
}

bool ListaPedidos::estaVacia() {
    return cabecera == NULL;
}

NodoPedido* ListaPedidos::extraer(const char* id_pedido) {
    if (cabecera == NULL) return NULL;

    NodoPedido* encontrado = NULL;

    if (strcmp(cabecera->datos.id_pedido, id_pedido) == 0) {
        encontrado = cabecera;
        cabecera = cabecera->sig;
        if (cabecera == NULL) cola = NULL;
        encontrado->sig = NULL;
        return encontrado;
    }

    NodoPedido* actual = cabecera;
    while (actual->sig != NULL && strcmp(actual->sig->datos.id_pedido, id_pedido) != 0) {
        actual = actual->sig;
    }

    if (actual->sig != NULL) {
        encontrado = actual->sig;
        actual->sig = encontrado->sig;
        if (actual->sig == NULL) cola = actual;
        encontrado->sig = NULL;
        return encontrado;
    }

    return NULL;
}

NodoPedido* ListaPedidos::getCabecera() {
    return cabecera;
}

// --- Implementación ABBLibrerias ---

ABBLibrerias::ABBLibrerias() {
    raiz = NULL;
}

ABBLibrerias::~ABBLibrerias() {
    borrarTodo(raiz);
}

void ABBLibrerias::borrarTodo(NodoLibreria* nodo) {
    if (nodo != NULL) {
        borrarTodo(nodo->izq);
        borrarTodo(nodo->der);
        delete nodo; // Esto llama al destructor de ListaPedidos automáticamente
    }
}

void ABBLibrerias::insertar(Libreria lib) {
    insertar(raiz, lib);
}

void ABBLibrerias::insertar(NodoLibreria*& nodo, Libreria lib) {
    if (nodo == NULL) {
        nodo = new NodoLibreria;
        nodo->datos = lib; // Copia la librería (y su lista vacía)
        nodo->izq = NULL;
        nodo->der = NULL;
    } else if (lib.id_libreria < nodo->datos.id_libreria) {
        insertar(nodo->izq, lib);
    } else if (lib.id_libreria > nodo->datos.id_libreria) {
        insertar(nodo->der, lib);
    }
    // Si el ID es igual, no hacemos nada (no duplicados)
}

void ABBLibrerias::borrar(int id_libreria) {
    borrar(raiz, id_libreria);
}

void ABBLibrerias::borrar(NodoLibreria*& nodo, int id) {
    if (nodo == NULL) return;

    if (id < nodo->datos.id_libreria) {
        borrar(nodo->izq, id);
    } else if (id > nodo->datos.id_libreria) {
        borrar(nodo->der, id);
    } else {
        // Nodo encontrado
        if (nodo->izq == NULL && nodo->der == NULL) {
            delete nodo;
            nodo = NULL;
        } else if (nodo->izq == NULL) {
            NodoLibreria* temp = nodo;
            nodo = nodo->der;
            delete temp;
        } else if (nodo->der == NULL) {
            NodoLibreria* temp = nodo;
            nodo = nodo->izq;
            delete temp;
        } else {
            // Dos hijos: buscar el menor del subárbol derecho
            NodoLibreria* minRight = nodo->der;
            while (minRight->izq != NULL) minRight = minRight->izq;

            // Copiar datos básicos
            nodo->datos.id_libreria = minRight->datos.id_libreria;
            strcpy(nodo->datos.localidad, minRight->datos.localidad);

            // --- TRUCO PARA COPIAR LA LISTA ---
            // Como no podemos asignar listas directamente sin un operador= seguro,
            // vaciamos la lista del nodo actual y copiamos los elementos del sucesor.

            // 1. Vaciar lista actual
            while (!nodo->datos.pedidos.estaVacia()) {
                NodoPedido* temp = nodo->datos.pedidos.getCabecera();
                nodo->datos.pedidos.borrar(temp->datos.id_pedido);
            }

            // 2. Copiar pedidos del sucesor (minRight)
            NodoPedido* aux = minRight->datos.pedidos.getCabecera();
            while(aux != NULL) {
                nodo->datos.pedidos.insertar(aux->datos);
                aux = aux->sig;
            }

            // 3. Borrar el sucesor original
            borrar(nodo->der, minRight->datos.id_libreria);
        }
    }
}

Libreria* ABBLibrerias::buscar(int id_libreria) {
    NodoLibreria* res = buscar(raiz, id_libreria);
    if (res != NULL) return &res->datos;
    return NULL;
}

NodoLibreria* ABBLibrerias::buscar(NodoLibreria* nodo, int id) {
    if (nodo == NULL) return NULL;
    if (id == nodo->datos.id_libreria) return nodo;
    if (id < nodo->datos.id_libreria) return buscar(nodo->izq, id);
    return buscar(nodo->der, id);
}

void ABBLibrerias::mostrar() {
    mostrar(raiz);
}

// Función auxiliar interna recursiva
void ABBLibrerias::mostrar(NodoLibreria* nodo) {
    if (nodo != NULL) {
        mostrar(nodo->izq);

        // Calculamos cuántos pedidos tiene esta librería
        int numPedidos = nodo->datos.pedidos.getNumeroPedidos();

        cout << "ID: " << left << setw(5) << nodo->datos.id_libreria
             << " Localidad: " << setw(15) << nodo->datos.localidad
             << " Num Pedidos: " << numPedidos << endl;

        mostrar(nodo->der);
    }
}

void ABBLibrerias::distribuirPedido(Pedido p) {
    distribuir(raiz, p);
}

void ABBLibrerias::distribuir(NodoLibreria* nodo, Pedido p) {
    if (nodo == NULL) return;

    if (p.id_libreria == nodo->datos.id_libreria) {
        nodo->datos.pedidos.insertar(p);
    } else if (p.id_libreria < nodo->datos.id_libreria) {
        distribuir(nodo->izq, p);
    } else {
        distribuir(nodo->der, p);
    }
}

void ABBLibrerias::mostrarEstadisticas() {
    int maxVentas = -1;
    char nombreMax[20] = "N/A";
    estadisticas(raiz, maxVentas, nombreMax);
    cout << "Libreria con mas ventas: " << nombreMax << " (" << maxVentas << " pedidos)" << endl;
}

void ABBLibrerias::estadisticas(NodoLibreria* nodo, int& maxVentas, char* nombreMaxVentas) {
    if (nodo != NULL) {
        estadisticas(nodo->izq, maxVentas, nombreMaxVentas);

        // Contar pedidos
        int count = 0;
        NodoPedido* aux = nodo->datos.pedidos.getCabecera();
        while(aux != NULL) {
            count++;
            aux = aux->sig;
        }

        if (count > maxVentas) {
            maxVentas = count;
            strcpy(nombreMaxVentas, nodo->datos.localidad);
        }

        estadisticas(nodo->der, maxVentas, nombreMaxVentas);
    }
}

bool ABBLibrerias::estaVacia() {
    return raiz == NULL;
}

// Búsqueda global de pedidos
Pedido* ABBLibrerias::buscarPedido(const char* id_pedido) {
    return buscarPedido(raiz, id_pedido);
}

Pedido* ABBLibrerias::buscarPedido(NodoLibreria* nodo, const char* id_pedido) {
    if (nodo == NULL) return NULL;

    NodoPedido* found = nodo->datos.pedidos.buscar(id_pedido);
    if (found != NULL) return &found->datos;

    Pedido* res = buscarPedido(nodo->izq, id_pedido);
    if (res != NULL) return res;

    return buscarPedido(nodo->der, id_pedido);
}

bool ABBLibrerias::borrarPedido(const char* id_pedido) {
    return borrarPedido(raiz, id_pedido);
}

bool ABBLibrerias::borrarPedido(NodoLibreria* nodo, const char* id_pedido) {
    if (nodo == NULL) return false;

    NodoPedido* found = nodo->datos.pedidos.buscar(id_pedido);
    if (found != NULL) {
        nodo->datos.pedidos.borrar(id_pedido);
        return true;
    }

    if (borrarPedido(nodo->izq, id_pedido)) return true;
    return borrarPedido(nodo->der, id_pedido);
}
