#include "enrutador.h"
#include <iostream>
#include <map>
#include <queue>
#include <climits>
#include <algorithm>

const int INFINITO = INT_MAX;

Enrutador::Enrutador(const std::string& id) : id(id) {}

void Enrutador::actualizarTablaEnrutamiento(const std::map<std::string, Enrutador*>& enrutadores) {
    std::map<std::string, int> distancia;
    std::map<std::string, std::string> predecesor;
    std::priority_queue<std::pair<int, std::string>,
                        std::vector<std::pair<int, std::string>>,
                        std::greater<std::pair<int, std::string>>> cola;

    // Inicialización
    for (const auto& par : enrutadores) {
        distancia[par.first] = INFINITO;
        predecesor[par.first] = "";
    }
    distancia[id] = 0;
    cola.push({0, id});

    // Algoritmo de Dijkstra
    while (!cola.empty()) {
        std::string actual = cola.top().second;
        int distActual = cola.top().first;
        cola.pop();

        if (distActual > distancia[actual]) continue;

        for (const auto& vecino : enrutadores.at(actual)->enlacesDirectos) {
            std::string nodoVecino = vecino.first;
            int costo = vecino.second;

            if (distancia[actual] + costo < distancia[nodoVecino]) {
                distancia[nodoVecino] = distancia[actual] + costo;
                predecesor[nodoVecino] = actual;
                cola.push({distancia[nodoVecino], nodoVecino});
            }
        }
    }

    // Actualizar tablas
    tablaEnrutamiento = distancia;
    siguienteSalto.clear();
    for (const auto& par : predecesor) {
        if (!par.second.empty()) {
            std::string temp = par.first;
            while (predecesor[temp] != id && !predecesor[temp].empty()) {
                temp = predecesor[temp];
            }
            siguienteSalto[par.first] = temp;
        }
    }
}

void Enrutador::establecerEnlaceDirecto(const std::string& destino, int costo) {
    enlacesDirectos[destino] = costo;
}

void Enrutador::eliminarEnlaceDirecto(const std::string& destino) {
    enlacesDirectos.erase(destino);
}

int Enrutador::obtenerCosto(const std::string& destino) const {
    auto it = tablaEnrutamiento.find(destino);
    return (it != tablaEnrutamiento.end()) ? it->second : INFINITO;
}

std::string Enrutador::obtenerSiguienteSalto(const std::string& destino) const {
    auto it = siguienteSalto.find(destino);
    return (it != siguienteSalto.end()) ? it->second : "";
}

void Enrutador::imprimirTablaEnrutamiento() const {
    std::cout << "\nTabla de enrutamiento para " << id << ":\n";
    std::cout << "Destino\tCosto\tSiguiente Salto\n";
    for (const auto& entrada : tablaEnrutamiento) {
        std::cout << entrada.first << "\t";
        if (entrada.second == INFINITO) {
            std::cout << "INF";
        } else {
            std::cout << entrada.second;
        }
        std::cout << "\t" << (siguienteSalto.count(entrada.first) ? siguienteSalto.at(entrada.first) : "-") << "\n";
    }
}

const std::map<std::string, int>& Enrutador::obtenerEnlacesDirectos() const {
    return enlacesDirectos;
}

const std::string& Enrutador::obtenerId() const {
    return id;
}
