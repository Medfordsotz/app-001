#ifndef ENUMS_H
#define ENUMS_H

#include <string>


// ---------------------------
// Roles de usuario
// ---------------------------
enum class Rol {
    CLIENTE = 0,
    ADMINISTRADOR = 1,
    CONTROLADOR = 2,
    MENSAJERO = 3
};

// ---------------------------
// Estados del paquete
// ---------------------------
enum class EstadoPaquete {
    SOLICITADO = 0,
    EN_RECOLECCION,
    EN_TRANSITO,
    ENTREGADO,
    CANCELADO
};

// ---------------------------
// Estados del pago
// ---------------------------
enum class EstadoPago {
    PENDIENTE = 0,
    COMPLETADO,
    RECHAZADO
};

// ---------------------------
// Constantes generales
// ---------------------------
inline constexpr float PESO_MAXIMO             = 15.0f;
inline constexpr float TARIFA_BASE             = 25.0f;
inline constexpr float MULTIPLICADOR_NOCTURNO  = 2.0f;
inline constexpr int   HORA_INICIO_DIURNO      = 8;
inline constexpr int   HORA_FIN_DIURNO         = 18;

inline constexpr int ID_SIN_MENSAJERO = -1;

//
// ============================================================
// FUNCIONES AUXILIARES DE TEXTO
// ============================================================
//

// ---- Roles ----
inline std::string rolToString(Rol rol) {
    switch (rol) {
        case Rol::CLIENTE:        return "Cliente";
        case Rol::ADMINISTRADOR:  return "Administrador";
        case Rol::CONTROLADOR:    return "Controlador";
        case Rol::MENSAJERO:      return "Mensajero";
        default:                  return "Desconocido";
    }
}

// ---- Estado del Paquete ----
inline std::string estadoPaqueteToString(EstadoPaquete estado) {
    switch (estado) {
        case EstadoPaquete::SOLICITADO:     return "Solicitado";
        case EstadoPaquete::EN_RECOLECCION: return "En Recolección";
        case EstadoPaquete::EN_TRANSITO:    return "En Tránsito";
        case EstadoPaquete::ENTREGADO:      return "Entregado";
        case EstadoPaquete::CANCELADO:      return "Cancelado";
        default:                            return "Desconocido";
    }
}

// ---- Estado del Pago ----
inline std::string estadoPagoToString(EstadoPago estado) {
    switch (estado) {
        case EstadoPago::PENDIENTE:  return "Pendiente";
        case EstadoPago::COMPLETADO: return "Completado";
        case EstadoPago::RECHAZADO:  return "Rechazado";
        default:                     return "Desconocido";
    }
}

#endif 
// ENUMS_H
