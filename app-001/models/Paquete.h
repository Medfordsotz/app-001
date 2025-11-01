#ifndef PAQUETE_H
#define PAQUETE_H

#include <string>
#include <iostream>
#include <iomanip>
#include "Enums.h"  

class Paquete {
private:
    int id;
    float peso;
    std::string descripcion;
    std::string dirOrigen;
    std::string dirDestino;
    std::string nombreRemitente;
    std::string nombreDestinatario;
    std::string telefonoDestinatario;

   
    EstadoPaquete estado;

public:
    // ----------------------------
    // Constructor
    // ----------------------------
    Paquete(int id,
            float peso,
            const std::string& desc,
            const std::string& origen,
            const std::string& destino)
        : id(id),
          peso(peso),
          descripcion(desc),
          dirOrigen(origen),
          dirDestino(destino),
          nombreRemitente(),
          nombreDestinatario(),
          telefonoDestinatario(),
          estado(EstadoPaquete::SOLICITADO) {}

    // ----------------------------
    // Getters (evitar copia)
    // ----------------------------
    int getId() const { return id; }
    float getPeso() const { return peso; }

    const std::string& getDescripcion() const { return descripcion; }
    const std::string& getDirOrigen() const { return dirOrigen; }
    const std::string& getDirDestino() const { return dirDestino; }
    const std::string& getNombreRemitente() const { return nombreRemitente; }
    const std::string& getNombreDestinatario() const { return nombreDestinatario; }
    const std::string& getTelefonoDestinatario() const { return telefonoDestinatario; }

    EstadoPaquete getEstado() const { return estado; }

    // ----------------------------
    // Setters
    // ----------------------------
    void setDescripcion(const std::string& desc) { descripcion = desc; }
    void setDirOrigen(const std::string& origen) { dirOrigen = origen; }
    void setDirDestino(const std::string& destino) { dirDestino = destino; }

    void setNombreRemitente(const std::string& nombre) { nombreRemitente = nombre; }
    void setNombreDestinatario(const std::string& nombre) { nombreDestinatario = nombre; }
    void setTelefonoDestinatario(const std::string& tel) { telefonoDestinatario = tel; }

    void setEstado(EstadoPaquete nuevoEstado) { estado = nuevoEstado; }

    // ----------------------------
    // Validación
    // ----------------------------
    bool validarPeso() const {
        return peso > 0.0f && peso <= PESO_MAXIMO;
    }

    // ----------------------------
    // Mostrar información
    // ----------------------------
    void mostrarInfo() const {
        std::cout << "\n┌─── PAQUETE #" << id << " ───────────────────────┐\n";
        std::cout << "│ Peso: " << std::fixed << std::setprecision(2)
                  << peso << " lbs\n";
        std::cout << "│ Descripción: " << descripcion << "\n";
        std::cout << "│ Estado: " << estadoPaqueteToString(estado) << "\n";
        std::cout << "├─── Origen ────────────────────────────┤\n";
        std::cout << "│ " << dirOrigen << "\n";
        if (!nombreRemitente.empty()) {
            std::cout << "│ Remitente: " << nombreRemitente << "\n";
        }
        std::cout << "├─── Destino ───────────────────────────┤\n";
        std::cout << "│ " << dirDestino << "\n";
        if (!nombreDestinatario.empty()) {
            std::cout << "│ Destinatario: " << nombreDestinatario << "\n";
        }
        if (!telefonoDestinatario.empty()) {
            std::cout << "│ Teléfono: " << telefonoDestinatario << "\n";
        }
        std::cout << "└───────────────────────────────────────┘\n";
    }

    void mostrarResumen() const {
        std::cout << "[Paquete #" << id << "] " << std::fixed << std::setprecision(2)
                  << peso << " lbs - "
                  << estadoPaqueteToString(estado) << "\n";
        std::cout << "  De: " << dirOrigen << "\n";
        std::cout << "  A:  " << dirDestino << "\n";
    }
};

#endif // PAQUETE_H
