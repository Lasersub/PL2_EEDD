#include "ccontrol.h"

// ============================================================================
// FUNCIONES AUXILIARES (GENERADORES ALEATORIOS)
// ============================================================================
// Estas funciones no suelen ser el foco del examen, pero es bueno saber que
// usan rand() y sprintf para formatear cadenas (ej: P00012).

int generarIdLibreria() {
    return rand() % 900 + 100; // Genera numero entre 100 y 999
}

void generarIdPedido(char* buffer) {
    // sprintf imprime dentro de un string en lugar de en la consola.
    // %05d rellena con ceros a la izquierda si el numero es pequeño.
    sprintf(buffer, "P%05d", rand() % 100000);
}

void generarCodLibro(char* buffer) {
    char letra = 'A' + rand() % 26; // Genera una letra mayuscula aleatoria
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

// Crea un objeto Pedido y rellena sus campos llamando a las funciones de arriba
Pedido generarPedidoAleatorio() {
    Pedido p;
    p.id_libreria = 0; // Se asignará luego en el main al asociarlo a una librería
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
    // IMPORTANTE: No hace falta inicializar 'l.pedidos' aquí porque
    // el constructor de la clase ListaPedidos se llama automáticamente.
    return l;
}

// ============================================================================
// IMPLEMENTACIÓN DE LISTA ENLAZADA (ListaPedidos)
// ============================================================================
// CLAVE EXAMEN: Manejo de punteros 'sig', 'cabecera' y 'cola'.

ListaPedidos::ListaPedidos() {
    // Estado inicial: lista vacía
    cabecera = NULL;
    cola = NULL;
}

// DESTRUCTOR: Libera memoria. Fundamental para evitar fugas (Memory Leaks).
ListaPedidos::~ListaPedidos() {
    NodoPedido* aux = cabecera;
    while (aux != NULL) {
        NodoPedido* borrar = aux; // Guardamos puntero al nodo actual
        aux = aux->sig;           // Avanzamos aux antes de borrar
        delete borrar;            // Borramos el nodo guardado
    }
}

// INSERTAR AL FINAL (Cola)
// Complejidad O(1) gracias al puntero 'cola'.
void ListaPedidos::insertar(Pedido p) {
    NodoPedido* nuevo = new NodoPedido; // 1. Reservar memoria
    nuevo->datos = p;                   // 2. Copiar datos
    nuevo->sig = NULL;                  // 3. El nuevo será el último, así que apunta a NULL

    if (cabecera == NULL) {
        // Caso Lista Vacía: El nuevo es el primero y el último
        cabecera = nuevo;
        cola = nuevo;
    } else {
        // Caso Lista No Vacía:
        cola->sig = nuevo; // El que era último ahora apunta al nuevo
        cola = nuevo;      // Actualizamos el puntero cola al nuevo último
    }
}

// BORRAR ELEMENTO DE LISTA
// CLAVE EXAMEN: Hay dos casos, borrar el primero (cabecera) o uno intermedio/final.
void ListaPedidos::borrar(const char* id_pedido) {
    if (cabecera == NULL) return; // Lista vacía, nada que hacer

    // CASO 1: Borrar el primer nodo (Cabecera)
    if (strcmp(cabecera->datos.id_pedido, id_pedido) == 0) {
        NodoPedido* borrar = cabecera;
        cabecera = cabecera->sig;     // Movemos cabecera al siguiente
        if (cabecera == NULL) cola = NULL; // Si la lista quedó vacía, actualizamos cola
        delete borrar;                // Liberamos memoria
        return;
    }

    // CASO 2: Borrar nodo intermedio o final
    NodoPedido* actual = cabecera;
    // Buscamos el nodo ANTERIOR al que queremos borrar (actual->sig)
    while (actual->sig != NULL && strcmp(actual->sig->datos.id_pedido, id_pedido) != 0) {
        actual = actual->sig;
    }

    if (actual->sig != NULL) { // Si paramos porque lo encontramos (no porque llegamos al final)
        NodoPedido* borrar = actual->sig; // Este es el que queremos eliminar
        actual->sig = borrar->sig;        // Saltamos el nodo a borrar

        // Si borramos el último, hay que actualizar 'cola'
        if (actual->sig == NULL) cola = actual;

        delete borrar; // Liberamos memoria
    }
}

// BÚSQUEDA SECUENCIAL
NodoPedido* ListaPedidos::buscar(const char* id_pedido) {
    NodoPedido* actual = cabecera;
    while (actual != NULL) {
        if (strcmp(actual->datos.id_pedido, id_pedido) == 0) {
            return actual; // Retorna puntero al nodo encontrado
        }
        actual = actual->sig;
    }
    return NULL; // No encontrado
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

    // Formato de tabla para la lista
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

// EXTRAER: Similar a borrar, pero devuelve el nodo en vez de hacer 'delete'.
// Útil para MOVER pedidos sin copiar y borrar datos manualmente.
NodoPedido* ListaPedidos::extraer(const char* id_pedido) {
    if (cabecera == NULL) return NULL;

    NodoPedido* encontrado = NULL;

    // Caso: Extraer cabecera
    if (strcmp(cabecera->datos.id_pedido, id_pedido) == 0) {
        encontrado = cabecera;
        cabecera = cabecera->sig;
        if (cabecera == NULL) cola = NULL;
        encontrado->sig = NULL; // Desconectamos el nodo extraído
        return encontrado;
    }

    // Caso: Extraer resto
    NodoPedido* actual = cabecera;
    while (actual->sig != NULL && strcmp(actual->sig->datos.id_pedido, id_pedido) != 0) {
        actual = actual->sig;
    }

    if (actual->sig != NULL) {
        encontrado = actual->sig;
        actual->sig = encontrado->sig; // Puenteamos
        if (actual->sig == NULL) cola = actual; // Si extrajimos el último, actualizamos cola
        encontrado->sig = NULL; // Desconectamos
        return encontrado;
    }

    return NULL;
}

NodoPedido* ListaPedidos::getCabecera() {
    return cabecera;
}

// ============================================================================
// IMPLEMENTACIÓN DE ÁRBOL BINARIO DE BÚSQUEDA (ABBLibrerias)
// ============================================================================
// CLAVE EXAMEN: Recursividad y paso de punteros por referencia (NodoLibreria*&).

ABBLibrerias::ABBLibrerias() {
    raiz = NULL;
}

ABBLibrerias::~ABBLibrerias() {
    borrarTodo(raiz);
}

// Recorrido POST-ORDEN para borrar (Izq -> Der -> Raiz)
// Es necesario borrar los hijos antes que el padre.
void ABBLibrerias::borrarTodo(NodoLibreria* nodo) {
    if (nodo != NULL) {
        borrarTodo(nodo->izq);
        borrarTodo(nodo->der);
        delete nodo; // Al borrar el nodo, se invoca ~ListaPedidos() automáticamente
    }
}

// Interfaz pública (llama a la privada)
void ABBLibrerias::insertar(Libreria lib) {
    insertar(raiz, lib);
}

// Función recursiva de inserción
// IMPORTANTE: 'NodoLibreria*& nodo' -> Paso por referencia del puntero.
// Permite modificar 'raiz' o 'nodo->izq' directamente.
void ABBLibrerias::insertar(NodoLibreria*& nodo, Libreria lib) {
    if (nodo == NULL) {
        // Caso Base: Hemos llegado a un hueco libre, insertamos aquí.
        nodo = new NodoLibreria;
        nodo->datos = lib;
        nodo->izq = NULL;
        nodo->der = NULL;
    } else if (lib.id_libreria < nodo->datos.id_libreria) {
        // Si es menor, bajamos por la izquierda
        insertar(nodo->izq, lib);
    } else if (lib.id_libreria > nodo->datos.id_libreria) {
        // Si es mayor, bajamos por la derecha
        insertar(nodo->der, lib);
    }
    // Si es igual, no hacemos nada (evitamos duplicados)
}

void ABBLibrerias::borrar(int id_libreria) {
    borrar(raiz, id_libreria);
}

// CLAVE EXAMEN: Borrado en ABB (los 3 casos)
void ABBLibrerias::borrar(NodoLibreria*& nodo, int id) {
    if (nodo == NULL) return; // No encontrado

    if (id < nodo->datos.id_libreria) {
        borrar(nodo->izq, id);
    } else if (id > nodo->datos.id_libreria) {
        borrar(nodo->der, id);
    } else {
        // Nodo encontrado. Analizamos casos:

        // CASO 1: Nodo Hoja (sin hijos) -> Se borra directamente
        if (nodo->izq == NULL && nodo->der == NULL) {
            delete nodo;
            nodo = NULL; // Importante poner a NULL el puntero del padre
        }
        // CASO 2: Solo un hijo (Izquierdo o Derecho) -> El hijo reemplaza al padre
        else if (nodo->izq == NULL) {
            NodoLibreria* temp = nodo;
            nodo = nodo->der; // El padre ahora apunta al hijo derecho
            delete temp;
        } else if (nodo->der == NULL) {
            NodoLibreria* temp = nodo;
            nodo = nodo->izq; // El padre ahora apunta al hijo izquierdo
            delete temp;
        }
        // CASO 3: Dos hijos -> Sustitución
        else {
            // Buscamos el MENOR del subárbol DERECHO (Sucesor Inorden)
            NodoLibreria* minRight = nodo->der;
            while (minRight->izq != NULL) minRight = minRight->izq;

            // Copiamos datos del sucesor al nodo actual
            nodo->datos.id_libreria = minRight->datos.id_libreria;
            strcpy(nodo->datos.localidad, minRight->datos.localidad);

            // COMPLICACIÓN: Como tenemos una Lista dentro, no basta copiar punteros.
            // Hay que mover los datos de la lista del sucesor aquí.

            // 1. Limpiamos la lista actual
            while (!nodo->datos.pedidos.estaVacia()) {
                NodoPedido* temp = nodo->datos.pedidos.getCabecera();
                nodo->datos.pedidos.borrar(temp->datos.id_pedido);
            }

            // 2. Copiamos (duplicamos) los pedidos del sucesor
            NodoPedido* aux = minRight->datos.pedidos.getCabecera();
            while(aux != NULL) {
                nodo->datos.pedidos.insertar(aux->datos);
                aux = aux->sig;
            }

            // 3. Borramos el nodo sucesor original (que ahora está duplicado aquí)
            borrar(nodo->der, minRight->datos.id_libreria);
        }
    }
}

Libreria* ABBLibrerias::buscar(int id_libreria) {
    NodoLibreria* res = buscar(raiz, id_libreria);
    if (res != NULL) return &res->datos;
    return NULL;
}

// Búsqueda binaria recursiva
NodoLibreria* ABBLibrerias::buscar(NodoLibreria* nodo, int id) {
    if (nodo == NULL) return NULL; // No encontrado
    if (id == nodo->datos.id_libreria) return nodo; // Encontrado

    // Decisión binaria: izquierda o derecha
    if (id < nodo->datos.id_libreria) return buscar(nodo->izq, id);
    return buscar(nodo->der, id);
}

void ABBLibrerias::mostrar() {
    mostrar(raiz);
}

// Recorrido INORDEN (Izq -> Raiz -> Der)
// Imprime los elementos ordenados de menor a mayor ID.
void ABBLibrerias::mostrar(NodoLibreria* nodo) {
    if (nodo != NULL) {
        mostrar(nodo->izq);

        int numPedidos = nodo->datos.pedidos.getNumeroPedidos();
        cout << "ID: " << left << setw(5) << nodo->datos.id_libreria
             << " Localidad: " << setw(15) << nodo->datos.localidad
             << " Num Pedidos: " << numPedidos << endl;

        mostrar(nodo->der);
    }
}

// ============================================================================
// LÓGICA DE ESTADÍSTICAS (ACUMULADORES)
// ============================================================================

void ABBLibrerias::mostrarEstadisticas() {
    if (raiz == NULL) {
        cout << "El arbol esta vacio." << endl;
        return;
    }

    cout << "\n--- ESTADISTICAS TOTALES ---" << endl;

    // Variables locales para almacenar los resultados del cálculo
    int maxPedidosLib = -1;
    int idLibMax = 0;
    char locLibMax[20] = "N/A";

    // Arrays para conteo de frecuencias (HashMap manual)
    RegistroConteo listaLibros[100];
    int numLibros = 0;
    RegistroConteo listaMaterias[100];
    int numMaterias = 0;

    // Lanzamos la recolección recursiva que llenará las variables de arriba
    recolectarEstadisticas(raiz, maxPedidosLib, idLibMax, locLibMax, listaLibros, numLibros, listaMaterias, numMaterias);

    // --- MOSTRAR RESULTADOS ---
    // 1. Librería Top
    cout << "1. LIBRERIA CON MAS VENTAS:" << endl;
    cout << "   ID: " << idLibMax << " (" << locLibMax << ")" << endl;
    cout << "   Total Pedidos: " << maxPedidosLib << endl << endl;

    // 2. Libro Top (Busqueda del máximo en el array llenado)
    int maxUnidadesLibro = -1;
    char codLibroTop[7] = "N/A";
    for(int i = 0; i < numLibros; i++) {
        if (listaLibros[i].totalUnidades > maxUnidadesLibro) {
            maxUnidadesLibro = listaLibros[i].totalUnidades;
            strcpy(codLibroTop, listaLibros[i].clave);
        }
    }
    cout << "2. LIBRO MAS VENDIDO:" << endl;
    cout << "   Codigo: " << codLibroTop << endl;
    cout << "   Unidades totales: " << (maxUnidadesLibro == -1 ? 0 : maxUnidadesLibro) << endl << endl;

    // 3. Materia Top
    int maxUnidadesMateria = -1;
    char materiaTop[20] = "N/A";
    for(int i = 0; i < numMaterias; i++) {
        if (listaMaterias[i].totalUnidades > maxUnidadesMateria) {
            maxUnidadesMateria = listaMaterias[i].totalUnidades;
            strcpy(materiaTop, listaMaterias[i].clave);
        }
    }
    cout << "3. MATERIA MAS EXITOSA:" << endl;
    cout << "   Area: " << materiaTop << endl;
    cout << "   Unidades totales: " << (maxUnidadesMateria == -1 ? 0 : maxUnidadesMateria) << endl;
}

// Función recursiva que visita CADA nodo del árbol para sumar datos
void ABBLibrerias::recolectarEstadisticas(NodoLibreria* nodo,
                                          int& maxPedidosLib, int& idLibMax, char* locLibMax,
                                          RegistroConteo* libros, int& numLibros,
                                          RegistroConteo* materias, int& numMaterias) {
    if (nodo != NULL) {
        // 1. Visitar Izquierda
        recolectarEstadisticas(nodo->izq, maxPedidosLib, idLibMax, locLibMax, libros, numLibros, materias, numMaterias);

        // 2. PROCESAR NODO ACTUAL

        // A) ¿Es esta la librería con más pedidos?
        int totalPedidos = nodo->datos.pedidos.getNumeroPedidos();
        if (totalPedidos > maxPedidosLib) {
            maxPedidosLib = totalPedidos;
            idLibMax = nodo->datos.id_libreria;
            strcpy(locLibMax, nodo->datos.localidad);
        }

        // B) Recorrer la lista de pedidos de ESTA librería
        NodoPedido* aux = nodo->datos.pedidos.getCabecera();
        while (aux != NULL) {
            // Acumular LIBRO: Buscamos si ya existe en el array 'libros'
            bool encontradoLibro = false;
            for(int i=0; i<numLibros; i++) {
                if(strcmp(libros[i].clave, aux->datos.cod_libro) == 0) {
                    libros[i].totalUnidades += aux->datos.unidades;
                    encontradoLibro = true;
                    break;
                }
            }
            // Si no existe, lo añadimos nuevo al array
            if (!encontradoLibro) {
                strcpy(libros[numLibros].clave, aux->datos.cod_libro);
                libros[numLibros].totalUnidades = aux->datos.unidades;
                numLibros++;
            }

            // Acumular MATERIA: Mismo proceso
            bool encontradoMateria = false;
            for(int i=0; i<numMaterias; i++) {
                if(strcmp(materias[i].clave, aux->datos.materia) == 0) {
                    materias[i].totalUnidades += aux->datos.unidades;
                    encontradoMateria = true;
                    break;
                }
            }
            if (!encontradoMateria) {
                strcpy(materias[numMaterias].clave, aux->datos.materia);
                materias[numMaterias].totalUnidades = aux->datos.unidades;
                numMaterias++;
            }

            aux = aux->sig;
        }

        // 3. Visitar Derecha
        recolectarEstadisticas(nodo->der, maxPedidosLib, idLibMax, locLibMax, libros, numLibros, materias, numMaterias);
    }
}

// ============================================================================
// FUNCIONES DE BÚSQUEDA Y DISTRIBUCIÓN
// ============================================================================

void ABBLibrerias::distribuirPedido(Pedido p) {
    distribuir(raiz, p);
}

// Busca la librería correcta y le inserta el pedido
void ABBLibrerias::distribuir(NodoLibreria* nodo, Pedido p) {
    if (nodo == NULL) return;

    if (p.id_libreria == nodo->datos.id_libreria) {
        // ¡Encontrada! Insertamos en su lista interna.
        nodo->datos.pedidos.insertar(p);
    } else if (p.id_libreria < nodo->datos.id_libreria) {
        distribuir(nodo->izq, p);
    } else {
        distribuir(nodo->der, p);
    }
}

bool ABBLibrerias::estaVacia() {
    return raiz == NULL;
}

// BÚSQUEDA GLOBAL DE PEDIDO
// Recorre todo el árbol hasta encontrar el pedido.
Pedido* ABBLibrerias::buscarPedido(const char* id_pedido) {
    return buscarPedido(raiz, id_pedido);
}

// Versión recursiva: Preorden (Raiz -> Izq -> Der) para buscar rápido
Pedido* ABBLibrerias::buscarPedido(NodoLibreria* nodo, const char* id_pedido) {
    if (nodo == NULL) return NULL;

    // 1. Mirar si está en la lista de esta librería
    NodoPedido* found = nodo->datos.pedidos.buscar(id_pedido);
    if (found != NULL) return &found->datos;

    // 2. Buscar en subárbol izquierdo
    Pedido* res = buscarPedido(nodo->izq, id_pedido);
    if (res != NULL) return res;

    // 3. Buscar en subárbol derecho
    return buscarPedido(nodo->der, id_pedido);
}

bool ABBLibrerias::borrarPedido(const char* id_pedido) {
    return borrarPedido(raiz, id_pedido);
}

bool ABBLibrerias::borrarPedido(NodoLibreria* nodo, const char* id_pedido) {
    if (nodo == NULL) return false;

    // 1. Intentar borrar de la lista de esta librería
    NodoPedido* found = nodo->datos.pedidos.buscar(id_pedido);
    if (found != NULL) {
        nodo->datos.pedidos.borrar(id_pedido);
        return true;
    }

    // 2. Seguir buscando recursivamente
    if (borrarPedido(nodo->izq, id_pedido)) return true;
    return borrarPedido(nodo->der, id_pedido);
}
