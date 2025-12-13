#ifndef CCONTROL_H
#define CCONTROL_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iomanip>

using namespace std;

// Constantes
#define N_LIBRERIAS 10
#define N_PEDIDOS 30

// Estructuras
struct Pedido {
    int id_libreria;
    char id_pedido[7]; // P + 5 digitos
    char cod_libro[7]; // 3 dig + 1 char + 2 dig
    char materia[20];
    int unidades;
    char fecha_envio[11]; // DD-MM-2025
};

struct NodoPedido {
    Pedido datos;
    NodoPedido* sig;
};

class ListaPedidos {
private:
    NodoPedido* cabecera;
    NodoPedido* cola;

public:
    ListaPedidos();
    ~ListaPedidos();

    void insertar(Pedido p);
    void borrar(const char* id_pedido); // Usamos const char* por seguridad
    NodoPedido* buscar(const char* id_pedido);
    int getNumeroPedidos();
    void mostrar();
    bool estaVacia();
    NodoPedido* extraer(const char* id_pedido);
    NodoPedido* getCabecera();
};

struct Libreria {
    int id_libreria;
    char localidad[20];
    ListaPedidos pedidos;
};

struct NodoLibreria {
    Libreria datos;
    NodoLibreria* izq;
    NodoLibreria* der;
};

// Estructura auxiliar para contar estadísticas (Interna para el cálculo)
struct RegistroConteo {
    char clave[20]; // Servirá para cod_libro o materia
    int totalUnidades;
};

class ABBLibrerias {
private:
    NodoLibreria* raiz;

    // Helpers recursivos privados
    void insertar(NodoLibreria*& nodo, Libreria lib);
    void borrar(NodoLibreria*& nodo, int id);
    void mostrar(NodoLibreria* nodo);
    void borrarTodo(NodoLibreria* nodo);
    NodoLibreria* buscar(NodoLibreria* nodo, int id);
    void estadisticas(NodoLibreria* nodo, int& maxVentas, char* nombreMaxVentas);
    void distribuir(NodoLibreria* nodo, Pedido p);

    // Helpers de búsqueda global
    Pedido* buscarPedido(NodoLibreria* nodo, const char* id_pedido);
    bool borrarPedido(NodoLibreria* nodo, const char* id_pedido);

    // Helper de estadísticas
    void recolectarEstadisticas(NodoLibreria* nodo,
                                int& maxPedidosLib, int& idLibMax, char* locLibMax,
                                RegistroConteo* libros, int& numLibros,
                                RegistroConteo* materias, int& numMaterias);

public:
    ABBLibrerias();
    ~ABBLibrerias();

    void insertar(Libreria lib);
    void borrar(int id_libreria);
    Libreria* buscar(int id_libreria);
    void mostrar();
    void distribuirPedido(Pedido p);

    // Función para mostrar el reporte completo
    void mostrarEstadisticas();

    bool estaVacia();

    // Métodos globales para pedidos
    Pedido* buscarPedido(const char* id_pedido);
    bool borrarPedido(const char* id_pedido);
};

// Funciones Auxiliares Globales
int generarIdLibreria();
void generarIdPedido(char* buffer);
void generarCodLibro(char* buffer);
void generarMateria(char* buffer);
void generarFecha(char* buffer);
void generarLocalidad(char* buffer);
Pedido generarPedidoAleatorio();
Libreria generarLibreriaAleatoria();

#endif
