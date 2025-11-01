#ifndef SOLICITUD_CONTROLLER_H
#define SOLICITUD_CONTROLLER_H

#include <vector>
#include <string>
#include <optional>
#include "../models/Paquete.h"
#include "../models/Solicitud.h"
#include "../models/Enums.h"          
#include "../utils/CalculoTarifas.h"  

class SolicitudController {
private:
    std::vector<Paquete*>&   paquetes;
    std::vector<Solicitud*>& solicitudes;
    int& siguienteIdPaquete;
    int& siguienteIdSolicitud;
//para el peso
    static constexpr float PESO_MAXIMO_LBS = 15.0f;   

public:
    SolicitudController(std::vector<Paquete*>& paqs,
                        std::vector<Solicitud*>& sols,
                        int& nextIdPaq,
                        int& nextIdSol)
        : paquetes(paqs),
          solicitudes(sols),
          siguienteIdPaquete(nextIdPaq),
          siguienteIdSolicitud(nextIdSol) {}

    // Crea un paquete validando peso. 
    Paquete* crearPaquete(float pesoLbs,
                          const std::string& descripcion,
                          const std::string& dirOrigen,
                          const std::string& dirDestino)
    {
        if (pesoLbs <= 0.0f || pesoLbs > PESO_MAXIMO_LBS) {
            return nullptr;
        }
        auto* p = new Paquete(siguienteIdPaquete++,
                              pesoLbs, descripcion, dirOrigen, dirDestino);
        paquetes.push_back(p);
        return p;
    }

    // Crea una solicitud para un usuario dado.
    Solicitud* crearSolicitud(int idUsuario, Paquete* paquete) {
        if (!paquete) return nullptr;
        auto* s = new Solicitud(siguienteIdSolicitud++, idUsuario, paquete);
        
        solicitudes.push_back(s);
        return s;
    }

    // Buscar por ID
    Solicitud* buscarPorId(int idSolicitud) const {
        for (auto* s : solicitudes) {
            if (s && s->getId() == idSolicitud) return s;
        }
        return nullptr;
    }

    // Listar por estado (sin imprimir)
    std::vector<Solicitud*> obtenerPorEstado(EstadoPaquete estado) const {
        std::vector<Solicitud*> out;
        out.reserve(solicitudes.size());
        for (auto* s : solicitudes) {
            if (s && s->getEstado() == estado) out.push_back(s);
        }
        return out;
    }

    // Listar del usuario (sin imprimir)
    std::vector<Solicitud*> obtenerPorUsuario(int idUsuario) const {
        std::vector<Solicitud*> out;
        out.reserve(8);
        for (auto* s : solicitudes) {
            if (s && s->getIdUsuario() == idUsuario) out.push_back(s);
        }
        return out;
    }

    // Marcar estados (para vistas de mensajero/controlador)
    bool actualizarEstado(int idSolicitud, EstadoPaquete nuevoEstado) {
        auto* s = buscarPorId(idSolicitud);
        if (!s) return false;
        s->actualizarEstado(nuevoEstado);  
        return true;
    }

    // Helper rápido: ¿está sin asignar?
    static bool sinMensajero(const Solicitud* s) {
        if (!s) return false;
        return s->getIdMensajero() == -1; 
    }
};

#endif 
// SOLICITUD_CONTROLLER_H
