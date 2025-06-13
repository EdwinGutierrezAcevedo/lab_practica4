#ifndef RED_H
#define RED_H

#include "enrutador.h"
#include <string>
#include <vector>
#include <map>

class Red {
private:
    std::map<std::string, Enrutador*> enrutadores;

    void limpiarMemoria();
    void actualizarTablas();

public:
    ~Red();
    void agregarEnrutador(const std::string& id);
    void eliminarEnrutador(const std::string& id);
    void establecerEnlace(const std::string& origen, const std::string& destino, int costo, bool bidireccional = true);
    void eliminarEnlace(const std::string& origen, const std::string& destino, bool bidireccional = true);
    int obtenerCostoEnvio(const std::string& origen, const std::string& destino);
    std::vector<std::string> obtenerCaminoOptimo(const std::string& origen, const std::string& destino);
    void imprimirMatrizCostos() const;
    void imprimirEnrutadores() const;
    void imprimirTablasEnrutamiento();
    void generarRedAleatoria(int numEnrutadores, double probabilidadConexion, int costoMaximo = 10);
    void cargarDesdeArchivo(const std::string& nombreArchivo);
};

#endif // RED_H
