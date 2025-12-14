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
        cout << "|" << right << setw(11) << bufferPedidos[i].id_libreria << " "
             << "|" << left  << setw(10) << bufferPedidos[i].id_pedido
             << "|" << setw(10) << bufferPedidos[i].cod_libro
             << "|" << setw(15) << bufferPedidos[i].materia
             << "|" << right << setw(8) << bufferPedidos[i].unidades << " "
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
        cout << "9- Mostrar librerias inorden" << endl;
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
                bool idValido = false;

                // Bucle de validación para el ID
                do {
                    cout << "ID Libreria (3 cifras, 100-999): ";
                    if (!(cin >> l.id_libreria)) { // Validación de tipo (si mete letras)
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << " [ERROR] Entrada no numerica." << endl;
                        l.id_libreria = 0; // Forzar repetición
                    } else if (l.id_libreria < 100 || l.id_libreria > 999) { // Validación de rango
                        cout << " [ERROR] El ID debe estar entre 100 y 999." << endl;
                    } else {
                        idValido = true;
                    }
                } while (!idValido);

                // Verificamos si existe en el árbol
                if (arbolReal.buscar(l.id_libreria) != NULL) {
                    cout << " [ERROR] Ya existe una libreria con el ID " << l.id_libreria << "." << endl;
                } else {
                    cout << "Localidad (una sola palabra, use guiones bajos_si_necesario): ";
                    cin >> l.localidad;
                    arbolReal.insertar(l);
                    cout << "Libreria insertada correctamente." << endl;
                }
                break;
            }
            case 2: {
                // MOSTRAR LIBRERIAS DISPONIBLES
                cout << "\n--- Librerias Disponibles ---" << endl;
                arbolReal.mostrar();

                int id;
                cout << "ID Libreria a borrar: "; cin >> id;

                // Verificamos si existe antes de borrar ---
                if (arbolReal.buscar(id) != NULL) {
                    arbolReal.borrar(id);
                    cout << "Libreria con ID " << id << " borrada correctamente." << endl;
                } else {
                    cout << "ERROR: Librería con ID " << id << " no encontrada." << endl;
                }
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
                    // Mostrar ID en el error
                    cout << "ERROR: Libreria con ID " << id << " no encontrada." << endl;
                }
                break;
            }
            case 4: {
                char idPedido[10];
                cout << "ID Pedido a buscar: "; cin >> idPedido;
                Pedido* p = arbolReal.buscarPedido(idPedido);

                if (p != NULL) {
                    cout << "\n[EXITO] Pedido encontrado con los siguientes detalles:" << endl;
                    cout << "-----------------------------------------------------------------------------" << endl;

                    cout << "|" << left << setw(14) << "UBICACION"
                         << "|" << setw(10) << "ID Pedido"
                         << "|" << setw(10) << "Cod Libro"
                         << "|" << setw(15) << "Materia"
                         << "|" << setw(9)  << "Unidades"
                         << "|" << setw(12) << "Fecha" << "|" << endl;

                    cout << "-----------------------------------------------------------------------------" << endl;

                    cout << "| Lib: " << right << setw(7) << p->id_libreria << " "
                         << "|" << left  << setw(10) << p->id_pedido
                         << "|" << setw(10) << p->cod_libro
                         << "|" << setw(15) << p->materia
                         << "|" << right << setw(8) << p->unidades << " "
                         << "|" << left  << setw(12) << p->fecha_envio << "|" << endl;

                    cout << "-----------------------------------------------------------------------------" << endl;
                } else {
                    cout << "\n[ERROR] No se ha encontrado ningun pedido con el ID: " << idPedido << endl;
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
                char idPedido[10];
                cout << "ID Pedido a mover: "; cin >> idPedido;

                // 1. Buscamos el pedido globalmente para saber dónde está
                Pedido* pFound = arbolReal.buscarPedido(idPedido);

                if (pFound != NULL) {
                    int idOrigen = pFound->id_libreria;
                    cout << "Pedido encontrado en la libreria origen: " << idOrigen << endl;

                    // 2. Pedimos destino
                    cout << "\n--- Librerias Disponibles ---" << endl;
                    arbolReal.mostrar();

                    int idDestino;
                    cout << "ID Libreria Destino: "; cin >> idDestino;

                    if (idOrigen == idDestino) {
                        cout << "El pedido ya se encuentra en esa libreria." << endl;
                    } else {
                        Libreria* origen = arbolReal.buscar(idOrigen);
                        Libreria* destino = arbolReal.buscar(idDestino);

                        if (destino != NULL && origen != NULL) {
                            // Extraemos el nodo de la lista origen
                            NodoPedido* nodoP = origen->pedidos.extraer(idPedido);
                            if (nodoP != NULL) {
                                // Actualizamos y movemos
                                nodoP->datos.id_libreria = idDestino;
                                destino->pedidos.insertar(nodoP->datos);
                                delete nodoP;
                                cout << "Pedido movido exitosamente de " << idOrigen << " a " << idDestino << "." << endl;
                            } else {
                                cout << "Error al extraer el pedido." << endl;
                            }
                        } else {
                            if (destino == NULL) cout << "ERROR: Libreria Destino con ID " << idDestino << " no encontrada." << endl;
                        }
                    }
                } else {
                    cout << "ERROR: Pedido con ID " << idPedido << " no encontrado en ninguna libreria." << endl;
                }
                break;
            }
            case 7: {
                arbolReal.mostrarEstadisticas();
                break;
            }
            case 8: {
                cout << "Generando " << N_PEDIDOS << " nuevos pedidos..." << endl;

                // --- CABECERA DE LA TABLA (IGUAL QUE AL INICIO) ---
                cout << "---------------------------------------------------------------------------" << endl;
                cout << "|" << left << setw(12) << "ID Libreria"
                     << "|" << setw(10) << "ID Pedido"
                     << "|" << setw(10) << "Cod Libro"
                     << "|" << setw(15) << "Materia"
                     << "|" << setw(9)  << "Unidades"
                     << "|" << setw(12) << "Fecha" << "|" << endl;
                cout << "---------------------------------------------------------------------------" << endl;

                for (int i = 0; i < N_PEDIDOS; i++) {
                    Pedido p = generarPedidoAleatorio();
                    p.id_libreria = idsValidos[rand() % N_LIBRERIAS];

                    // --- IMPRIMIR FILA ---
                    cout << "|" << right << setw(11) << p.id_libreria << " "
                         << "|" << left  << setw(10) << p.id_pedido
                         << "|" << setw(10) << p.cod_libro
                         << "|" << setw(15) << p.materia
                         << "|" << right << setw(8) << p.unidades << " "
                         << "|" << left  << setw(12) << p.fecha_envio << "|" << endl;

                    arbolReal.distribuirPedido(p);
                }
                cout << "---------------------------------------------------------------------------" << endl;
                cout << "Pedidos distribuidos correctamente." << endl;
                break;
            }
            case 9: {
                cout << "\n--- Listado de Librerias (Inorden) ---" << endl;
                arbolReal.mostrar(); // Esta función ya implementa el recorrido inorden
                break;
            }
            case 0:
                cout << "\nSaliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }

        // Pausa para que el usuario pueda leer el resultado ---
        if (opcion != 0) {
            cout << "\nPresione ENTER para volver al menu...";
            cin.ignore(1000, '\n'); // Limpiar buffer previo
            cin.get(); // Esperar intro
            cout << endl;
        }

    } while (opcion != 0);

    return 0;
}

