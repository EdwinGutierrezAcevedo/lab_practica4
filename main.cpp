#include "red.h"
#include <iostream>
#include <vector>
#include <string>
const int INFINITO = INT_MAX;
void mostrarMenu() {
    std::cout << "\n=================================";
    std::cout << "\n   Simulador de Red de Enrutadores";
    std::cout << "\n=================================";
    std::cout << "\n1. Agregar enrutador";
    std::cout << "\n2. Eliminar enrutador";
    std::cout << "\n3. Establecer enlace";
    std::cout << "\n4. Eliminar enlace";
    std::cout << "\n5. Mostrar enrutadores";
    std::cout << "\n6. Mostrar matriz de costos";
    std::cout << "\n7. Mostrar tablas de enrutamiento";
    std::cout << "\n8. Consultar costo de envio";
    std::cout << "\n9. Consultar camino optimo";
    std::cout << "\n10. Generar red aleatoria";
    std::cout << "\n11. Cargar red desde archivo";
    std::cout << "\n0. Salir";
    std::cout << "\nSeleccione una opcion: ";
}

int main() {
    Red red;
    int opcion;

    do {
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer

        switch (opcion) {
        case 1: {
            std::string id;
            std::cout << "Ingrese ID del nuevo enrutador: ";
            std::cin >> id;
            red.agregarEnrutador(id);
            break;
        }
        case 2: {
            std::string id;
            std::cout << "Ingrese ID del enrutador a eliminar: ";
            std::cin >> id;
            red.eliminarEnrutador(id);
            break;
        }
        case 3: {
            std::string origen, destino;
            int costo;
            std::cout << "Ingrese enrutador origen: ";
            std::cin >> origen;
            std::cout << "Ingrese enrutador destino: ";
            std::cin >> destino;
            std::cout << "Ingrese costo del enlace: ";
            std::cin >> costo;
            red.establecerEnlace(origen, destino, costo);
            break;
        }
        case 4: {
            std::string origen, destino;
            std::cout << "Ingrese enrutador origen: ";
            std::cin >> origen;
            std::cout << "Ingrese enrutador destino: ";
            std::cin >> destino;
            red.eliminarEnlace(origen, destino);
            break;
        }
        case 5:
            red.imprimirEnrutadores();
            break;
        case 6:
            red.imprimirMatrizCostos();
            break;
        case 7:
            red.imprimirTablasEnrutamiento();
            break;
        case 8: {
            std::string origen, destino;
            std::cout << "Ingrese enrutador origen: ";
            std::cin >> origen;
            std::cout << "Ingrese enrutador destino: ";
            std::cin >> destino;
            int costo = red.obtenerCostoEnvio(origen, destino);
            if (costo == INFINITO) {
                std::cout << "No hay camino disponible de " << origen << " a " << destino << std::endl;
            } else {
                std::cout << "Costo de envio de " << origen << " a " << destino << ": " << costo << std::endl;
            }
            break;
        }
        case 9: {
            std::string origen, destino;
            std::cout << "Ingrese enrutador origen: ";
            std::cin >> origen;
            std::cout << "Ingrese enrutador destino: ";
            std::cin >> destino;
            std::vector<std::string> camino = red.obtenerCaminoOptimo(origen, destino);
            if (camino.empty()) {
                std::cout << "No hay camino disponible de " << origen << " a " << destino << std::endl;
            } else {
                std::cout << "Camino optimo: ";
                for (size_t i = 0; i < camino.size(); ++i) {
                    if (i != 0) std::cout << " -> ";
                    std::cout << camino[i];
                }
                std::cout << std::endl;
            }
            break;
        }
        case 10: {
            int numEnrutadores;
            double prob;
            std::cout << "Ingrese numero de enrutadores: ";
            std::cin >> numEnrutadores;
            std::cout << "Ingrese probabilidad de conexion (0-1): ";
            std::cin >> prob;
            red.generarRedAleatoria(numEnrutadores, prob);
            break;
        }
        case 11: {
            std::string nombreArchivo;
            std::cout << "Ingrese nombre del archivo: ";
            std::cin >> nombreArchivo;
            red.cargarDesdeArchivo(nombreArchivo);
            break;
        }
        case 0:
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opcion no valida. Intente nuevamente.\n";
        }
    } while (opcion != 0);

    return 0;
}
