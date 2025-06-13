#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <string>
#include <map>

class Enrutador {
private:
    std::string id;
    std::map<std::string, int> enlacesDirectos;
    std::map<std::string, int> tablaEnrutamiento;
    std::map<std::string, std::string> siguienteSalto;

public:
    Enrutador(const std::string& id);
    void actualizarTablaEnrutamiento(const std::map<std::string, Enrutador*>& enrutadores);
    void establecerEnlaceDirecto(const std::string& destino, int costo);
    void eliminarEnlaceDirecto(const std::string& destino);
    int obtenerCosto(const std::string& destino) const;
    std::string obtenerSiguienteSalto(const std::string& destino) const;
    void imprimirTablaEnrutamiento() const;
    const std::map<std::string, int>& obtenerEnlacesDirectos() const;
    const std::string& obtenerId() const;
};

#endif // ENRUTADOR_H
