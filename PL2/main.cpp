#include "ccontrol.h"

int main() {
    srand(time(NULL));

    ABBLibrerias arbolReal;
    Pedido bufferPedidos[N_PEDIDOS];
    int idsValidos[N_LIBRERIAS]; // Para guardar IDs válidos y asegurar que los pedidos van a sitios que existen

    // 1. Generar Librerias
    cout << "Generando " << N_LIBRERIAS << " librerias..." << endl;
    for (int i = 0; i < N_LIBRERIAS; i++) {
        Libreria l = generarLibreriaAleatoria();

        // Comprobar duplicados en el array local para no perder el ID
        bool unique = true;
        for(int j=0; j<i; j++) {
            if(idsValidos[j] == l.id_libreria) {
                unique = false;
                break;
            }
        }
        if(!unique) { i--; continue; } // Reintentar si sale repetido

        idsValidos[i] = l.id_libreria;
        arbolReal.insertar(l);
        cout << "Libreria generada: " << l.id_libreria << " - " << l.localidad << endl;
    }

    // 2. Generar Pedidos
    cout << "\nCreando los siguientes pedidos nuevos:" << endl;

    // --- CABECERA DE LA TABLA ---
    cout << "---------------------------------------------------------------------------" << endl;
    cout << "|" << left << setw(12) << "ID Libreria"
         << "|" << setw(10) << "ID Pedido"
         << "|" << setw(10) << "Cod Libro"
         << "|" << setw(15) << "Materia"
         << "|" << setw(9)  << "Unidades"
         << "|" << setw(12) << "Fecha" << "|" << endl;
    cout << "---------------------------------------------------------------------------" << endl;

    for (int i = 0; i < N_PEDIDOS; i++) {
        bufferPedidos[i] = generarPedidoAleatorio();
        // Asignar a una librería existente al azar
        bufferPedidos[i].id_libreria = idsValidos[rand() % N_LIBRERIAS];

        // --- FILA DE LA TABLA ---
        cout << "|" << right << setw(11) << bufferPedidos[i].id_libreria << " " // ID Libreria alineado a la dcha
             << "|" << left  << setw(10) << bufferPedidos[i].id_pedido
             << "|" << setw(10) << bufferPedidos[i].cod_libro
             << "|" << setw(15) << bufferPedidos[i].materia
             << "|" << right << setw(8) << bufferPedidos[i].unidades << " " // Unidades alineado a la dcha
             << "|" << left  << setw(12) << bufferPedidos[i].fecha_envio << "|" << endl;
    }
    cout << "---------------------------------------------------------------------------" << endl;

    // 3. Distribuir
    for (int i = 0; i < N_PEDIDOS; i++) {
        arbolReal.distribuirPedido(bufferPedidos[i]);
    }

    cout << "\n--- Estado Inicial del Arbol ---" << endl;
    arbolReal.mostrar();

    // 4. Menu
    int opcion;
    do {
        cout << "\n--- MENU ---" << endl;
        cout << "1- Insertar una libreria manual" << endl;
        cout << "2- Borrar una libreria" << endl;
        cout << "3- Mostrar pedidos de una libreria" << endl;
        cout << "4- Buscar pedido por ID" << endl;
        cout << "5- Borrar pedido" << endl;
        cout << "6- Mover pedido entre librerias" << endl;
        cout << "7- Estadisticas" << endl;
        cout << "8- Generar mas pedidos" << endl;
        cout << "0- Salir" << endl;
        cout << "Opcion: ";

        cin >> opcion;
        if(cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1: {
                Libreria l;
                cout << "ID Libreria (3 cifras): "; cin >> l.id_libreria;

                // --- MODIFICACION: Verificamos si existe ---
                if (arbolReal.buscar(l.id_libreria) != NULL) {
                    cout << "ERROR: Ya existe una libreria con el ID " << l.id_libreria << "." << endl;
                } else {
                    cout << "Localidad: "; cin >> l.localidad;
                    arbolReal.insertar(l);
                    cout << "Libreria insertada correctamente." << endl;
                }
                // -------------------------------------------
                break;
            }
            case 2: {
                // MOSTRAR LIBRERIAS DISPONIBLES
                cout << "\n--- Librerias Disponibles ---" << endl;
                arbolReal.mostrar();

                int id;
                cout << "ID Libreria a borrar: "; cin >> id;

                // --- MODIFICACION: Verificamos si existe antes de borrar ---
                if (arbolReal.buscar(id) != NULL) {
                    arbolReal.borrar(id);
                    cout << "Libreria con ID " << id << " borrada correctamente." << endl;
                } else {
                    cout << "ERROR: Librería con ID " << id << " no encontrada." << endl;
                }
                // -----------------------------------------------------------
                break;
            }
            case 3: {
                // MOSTRAR LIBRERIAS DISPONIBLES
                cout << "\n--- Librerias Disponibles ---" << endl;
                arbolReal.mostrar();

                int id;
                cout << "ID Libreria: "; cin >> id;
                Libreria* lib = arbolReal.buscar(id);
                if (lib != NULL) {
                    cout << "Pedidos de " << lib->localidad << ":" << endl;
                    lib->pedidos.mostrar();
                } else {
                    // --- MODIFICACION: Mostrar ID en el error ---
                    cout << "ERROR: Libreria con ID " << id << " no encontrada." << endl;
                }
                break;
            }
            case 4: {
                char idPedido[10];
                cout << "ID Pedido: "; cin >> idPedido;
                Pedido* p = arbolReal.buscarPedido(idPedido);
                if (p != NULL) {
                    cout << "Pedido encontrado: " << p->id_pedido
                          << " en libreria " << p->id_libreria << endl;
                } else {
                    cout << "Pedido no encontrado." << endl;
                }
                break;
            }
            case 5: {
                char idPedido[10];
                cout << "ID Pedido a borrar: "; cin >> idPedido;
                if (arbolReal.borrarPedido(idPedido)) {
                    cout << "Pedido borrado correctamente." << endl;
                } else {
                    cout << "Pedido no encontrado." << endl;
                }
                break;
            }
            case 6: {
                // MOSTRAR LIBRERIAS DISPONIBLES
                cout << "\n--- Librerias Disponibles ---" << endl;
                arbolReal.mostrar();

                int idOrigen, idDestino;
                char idPedido[10];
                cout << "ID Libreria Origen: "; cin >> idOrigen;
                cout << "ID Libreria Destino: "; cin >> idDestino;
                cout << "ID Pedido: "; cin >> idPedido;

                Libreria* origen = arbolReal.buscar(idOrigen);
                Libreria* destino = arbolReal.buscar(idDestino);

                if (origen && destino) {
                    NodoPedido* nodoP = origen->pedidos.extraer(idPedido);
                    if (nodoP != NULL) {
                        // Actualizar ID de librería en el pedido
                        nodoP->datos.id_libreria = idDestino;
                        destino->pedidos.insertar(nodoP->datos);
                        delete nodoP; // Borramos el nodo viejo, ya copiamos los datos al nuevo
                        cout << "Pedido movido." << endl;
                    } else {
                        cout << "Pedido no encontrado en origen." << endl;
                    }
                } else {
                    // --- MODIFICACION: Mostrar IDs en el error ---
                    if (!origen) cout << "ERROR: Libreria Origen con ID " << idOrigen << " no encontrada." << endl;
                    if (!destino) cout << "ERROR: Libreria Destino con ID " << idDestino << " no encontrada." << endl;
                }
                break;
            }
            case 7: {
                arbolReal.mostrarEstadisticas();
                break;
            }
            case 8: {
                cout << "Generando " << N_PEDIDOS << " nuevos pedidos..." << endl;
                for (int i = 0; i < N_PEDIDOS; i++) {
                    Pedido p = generarPedidoAleatorio();
                    // Intentamos asignarlo a una librería válida existente
                    p.id_libreria = idsValidos[rand() % N_LIBRERIAS];
                    arbolReal.distribuirPedido(p);
                }
                cout << "Pedidos distribuidos." << endl;
                break;
            }
            case 0:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);

    return 0;
}

