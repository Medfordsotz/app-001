#ifndef SOLICITUD_H
#define SOLICITUD_H

#include <iostream>
#include <iomanip>
#include "Paquete.h"
#include "Enums.h"                
#include "../utils/Utilidades.h"
#include "../utils/CalculoTarifas.h"

class Solicitud {
private:
    int id;
    int idUsuario;
    Paquete* paquete;             
    int idMensajero;
    Fecha fechaSolicitud;
    Fecha fechaRecoleccion;
    Fecha fechaEntrega;
    float montoTotal;
    EstadoPaquete estado;
    bool pagado;

public:
    // Constructor
    Solicitud(int id, int idUsuario, Paquete* paq)
        : id(id),
          idUsuario(idUsuario),
          paquete(paq),
          idMensajero(ID_SIN_MENSAJERO),
          fechaSolicitud(Utilidades::fechaActual()),
          fechaRecoleccion(),
          fechaEntrega(),
          montoTotal(0.0f),
          estado(EstadoPaquete::SOLICITADO),
          pagado(false)
    {
        calcularMonto();
        // sincroniza al crear Paquete,
        if (paquete) paquete->setEstado(estado);
    }

    //  NO borra 'paquete' (lo gestiona quien lo creó)
    ~Solicitud() = default;

    // ===== Getters =====
    int getId() const { return id; }
    int getIdUsuario() const { return idUsuario; }
    Paquete* getPaquete() const { return paquete; }
    int getIdMensajero() const { return idMensajero; }
    const Fecha& getFechaSolicitud() const { return fechaSolicitud; }
    const Fecha& getFechaRecoleccion() const { return fechaRecoleccion; }
    const Fecha& getFechaEntrega() const { return fechaEntrega; }
    float getMontoTotal() const { return montoTotal; }
    EstadoPaquete getEstado() const { return estado; }
    bool estaPagado() const { return pagado; }

    // ===== Acciones / Setters =====
    void asignarMensajero(int idMsg) {
        idMensajero = idMsg;
        actualizarEstado(EstadoPaquete::EN_RECOLECCION);
        fechaRecoleccion = Utilidades::fechaActual();
    }

    void actualizarEstado(EstadoPaquete nuevoEstado) {
        estado = nuevoEstado;
        if (paquete) paquete->setEstado(nuevoEstado);
        if (nuevoEstado == EstadoPaquete::ENTREGADO) {
            fechaEntrega = Utilidades::fechaActual();
        }
    }

    void marcarPagado() { pagado = true; }

    // Recalcular monto según reglas de tarifa
    void calcularMonto() {
        
        // float tarifa = CalculadoraTarifas::calcularTarifa(fechaSolicitud, paquete ? paquete->getPeso() : 0.0f);
        float tarifa = CalculadoraTarifas::calcularTarifa(fechaSolicitud);

        if (tarifa < 0.0f) {
            // Servicio no disponible (p.ej., domingo)
            montoTotal = 0.0f;
        } else {
            montoTotal = tarifa;
        }
    }

    // ===== Mostrar =====
    void mostrarInfo() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║      SOLICITUD #" << id << "\n";
        std::cout <<   "╠════════════════════════════════════════╣\n";
        std::cout <<   "║ Cliente ID: " << idUsuario << "\n";
        std::cout <<   "║ Fecha: " << fechaSolicitud.toString() << "\n";
        std::cout <<   "║ Estado: " << estadoPaqueteToString(estado) << "\n";
        std::cout <<   "║ Monto: Q" << std::fixed << std::setprecision(2) << montoTotal << "\n";
        std::cout <<   "║ Pago: " << (pagado ? "✓ Completado" : "○ Pendiente") << "\n";
        std::cout <<   "║ Mensajero: " << (idMensajero != ID_SIN_MENSAJERO ?
                          ("ID " + std::to_string(idMensajero)) : "Sin asignar") << "\n";
        std::cout <<   "╚════════════════════════════════════════╝\n";

        if (paquete) paquete->mostrarInfo();
    }

    void mostrarResumen() const {
        std::cout << "[" << id << "] " << fechaSolicitud.toString()
                  << " - " << estadoPaqueteToString(estado)
                  << " - Q" << std::fixed << std::setprecision(2) << montoTotal
                  << (pagado ? " [PAGADO]" : " [PENDIENTE]") << "\n";
    }
};

#endif // SOLICITUD_H
