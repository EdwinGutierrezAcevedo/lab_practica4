#include "red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <algorithm>

const int INFINITO = INT_MAX;

void Red::limpiarMemoria() {
    for (auto& par : enrutadores) {
        delete par.second;
    }
    enrutadores.clear();
}

Red::~Red() {
    limpiarMemoria();
}

void Red::agregarEnrutador(const std::string& id) {
    if (enrutadores.find(id) == enrutadores.end()) {
        enrutadores[id] = new Enrutador(id);
        std::cout << "Enrutador " << id << " agregado exitosamente.\n";
    } else {
        std::cout << "El enrutador " << id << " ya existe.\n";
    }
}

void Red::eliminarEnrutador(const std::string& id) {
    if (enrutadores.find(id) != enrutadores.end()) {
        delete enrutadores[id];
        enrutadores.erase(id);

        for (auto& par : enrutadores) {
            par.second->eliminarEnlaceDirecto(id);
        }
        actualizarTablas();
        std::cout << "Enrutador " << id << " eliminado exitosamente.\n";
    } else {
        std::cout << "El enrutador " << id << " no existe.\n";
    }
}

void Red::establecerEnlace(const std::string& origen, const std::string& destino, int costo, bool bidireccional) {
    if (enrutadores.count(origen) && enrutadores.count(destino)) {
        enrutadores[origen]->establecerEnlaceDirecto(destino, costo);
        if (bidireccional) {
            enrutadores[destino]->establecerEnlaceDirecto(origen, costo);
        }
        actualizarTablas();
        std::cout << "Enlace entre " << origen << " y " << destino << " establecido con costo " << costo << ".\n";
    } else {
        std::cout << "Error: Uno o ambos enrutadores no existen.\n";
    }
}

void Red::eliminarEnlace(const std::string& origen, const std::string& destino, bool bidireccional) {
    if (enrutadores.count(origen) && enrutadores.count(destino)) {
        enrutadores[origen]->eliminarEnlaceDirecto(destino);
        if (bidireccional) {
            enrutadores[destino]->eliminarEnlaceDirecto(origen);
        }
        actualizarTablas();
        std::cout << "Enlace entre " << origen << " y " << destino << " eliminado.\n";
    } else {
        std::cout << "Error: Uno o ambos enrutadores no existen.\n";
    }
}

void Red::actualizarTablas() {
    for (auto& par : enrutadores) {
        par.second->actualizarTablaEnrutamiento(enrutadores);
    }
    std::cout << "Tablas de enrutamiento actualizadas.\n";
}

int Red::obtenerCostoEnvio(const std::string& origen, const std::string& destino) {
    if (enrutadores.count(origen)) {
        return enrutadores.at(origen)->obtenerCosto(destino);
    }
    return INFINITO;
}

std::vector<std::string> Red::obtenerCaminoOptimo(const std::string& origen, const std::string& destino) {
    std::vector<std::string> camino;
    if (!enrutadores.count(origen) || !enrutadores.count(destino)) {
        return camino;
    }

    std::string actual = origen;
    while (actual != destino && !actual.empty()) {
        camino.push_back(actual);
        actual = enrutadores.at(actual)->obtenerSiguienteSalto(destino);
    }

    if (!actual.empty()) {
        camino.push_back(destino);
    }

    return camino;
}

void Red::imprimirMatrizCostos() const {
    if (enrutadores.empty()) {
        std::cout << "No hay enrutadores en la red.\n";
        return;
    }

    std::vector<std::string> ids;
    for (const auto& par : enrutadores) {
        ids.push_back(par.first);
    }
    std::sort(ids.begin(), ids.end());

    // Encabezado
    std::cout << "\nMatriz de Costos:\n     ";
    for (const std::string& id : ids) {
        std::cout << std::setw(6) << id;
    }
    std::cout << "\n";

    // Filas
    for (const std::string& origen : ids) {
        std::cout << std::setw(5) << origen;
        for (const std::string& destino : ids) {
            int costo = enrutadores.at(origen)->obtenerCosto(destino);
            if (costo == INFINITO) {
                std::cout << std::setw(6) << "INF";
            } else {
                std::cout << std::setw(6) << costo;
            }
        }
        std::cout << "\n";
    }
}

void Red::imprimirEnrutadores() const {
    if (enrutadores.empty()) {
        std::cout << "No hay enrutadores en la red.\n";
        return;
    }

    std::cout << "\nEnrutadores en la red (" << enrutadores.size() << "):\n";
    for (const auto& par : enrutadores) {
        std::cout << "- " << par.first << "\n";
    }
}

void Red::imprimirTablasEnrutamiento() {
    if (enrutadores.empty()) {
        std::cout << "No hay enrutadores en la red.\n";
        return;
    }

    std::cout << "\nTablas de Enrutamiento:\n";
    for (const auto& par : enrutadores) {
        par.second->imprimirTablaEnrutamiento();
    }
}

void Red::generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo) {
    if (numEnrutadores <= 0) {
        std::cout << "Numero de enrutadores debe ser positivo.\n";
        return;
    }

    limpiarMemoria();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> costo_dist(1, costoMaximo);
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);

    // Crear enrutadores
    for (int i = 0; i < numEnrutadores; ++i) {
        std::string id = "R" + std::to_string(i + 1);
        agregarEnrutador(id);
    }

    // Crear conexiones aleatorias
    std::vector<std::string> ids;
    for (const auto& par : enrutadores) {
        ids.push_back(par.first);
    }

    for (int i = 0; i < ids.size(); ++i) {
        for (int j = i + 1; j < ids.size(); ++j) {
            if (prob_dist(gen) < probabilidadConexion) {
                int costo = costo_dist(gen);
                establecerEnlace(ids[i], ids[j], costo);
            }
        }
    }

    std::cout << "Red aleatoria generada con " << numEnrutadores
              << " enrutadores y probabilidad de conexion " << probabilidadConexion << ".\n";
}

void Red::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    limpiarMemoria();
    std::string linea;
    int contador = 0;

    while (std::getline(archivo, linea)) {
        // Saltar líneas vacías o comentarios
        if (linea.empty() || linea[0] == '#') continue;

        std::istringstream iss(linea);
        std::string origen, destino;
        int costo;

        if (iss >> origen >> destino >> costo) {
            // Asegurarse que los enrutadores existan
            if (enrutadores.find(origen) == enrutadores.end()) {
                agregarEnrutador(origen);
            }
            if (enrutadores.find(destino) == enrutadores.end()) {
                agregarEnrutador(destino);
            }
            establecerEnlace(origen, destino, costo);
            contador++;
        }
    }

    std::cout << "Red cargada desde archivo. " << contador << " enlaces procesados.\n";
    archivo.close();
}
