#ifndef CONTROLADOR_CONTROLLER_H
#define CONTROLADOR_CONTROLLER_H

#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include "../models/Enums.h"     
#include "../models/Solicitud.h"
#include "../models/Mensaje.h"
#include "../models/Usuario.h"

class ControladorController {
private:
    std::vector<Solicitud*>& solicitudes;
    std::vector<Mensaje*>& mensajes;
    std::vector<Usuario*>& usuarios;
    int& siguienteIdMensaje;

   
    static constexpr int NO_MENSAJERO = -1;

    Solicitud* encontrarSolicitud(int idSolicitud) const {
        for (auto* s : solicitudes) {
            if (s && s->getId() == idSolicitud) return s;
        }
        return nullptr;
    }

    Usuario* encontrarUsuario(int idUsuario) const {
        for (auto* u : usuarios) {
            if (u && u->getId() == idUsuario) return u;
        }
        return nullptr;
    }

    Usuario* encontrarMensajeroActivo(int idMensajero) const {
        for (auto* u : usuarios) {
            if (u && u->getId() == idMensajero &&
                u->getRol() == Rol::MENSAJERO && u->estaActivo()) {
                return u;
            }
        }
        return nullptr;
    }

public:

    ControladorController(std::vector<Solicitud*>& sols,
                          std::vector<Mensaje*>& msgs,
                          std::vector<Usuario*>& users,
                          int& nextIdMsg)
        : solicitudes(sols), mensajes(msgs),
          usuarios(users), siguienteIdMensaje(nextIdMsg) {}

    // Listar mensajeros disponibles (activos). 
    int listarMensajeros() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║     MENSAJEROS DISPONIBLES             ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n\n";

        int cont = 0;
        for (const auto* usuario : usuarios) {
            if (usuario &&
                usuario->getRol() == Rol::MENSAJERO &&
                usuario->estaActivo()) {
                usuario->mostrarResumen();
                ++cont;
            }
        }
        if (cont == 0) {
            std::cout << "No hay mensajeros registrados.\n";
        }
        return cont;
    }

    // Asignar mensajero a solicitud
    bool asignarMensajero(int idSolicitud, int idMensajero, int idControlador) {
        
        Usuario* controlador = encontrarUsuario(idControlador);
        if (!controlador || controlador->getRol() != Rol::CONTROLADOR || !controlador->estaActivo()) {
            std::cout << "✗ El usuario que asigna no es un controlador válido/activo.\n";
            return false;
        }

        // Buscar solicitud
        Solicitud* solicitud = encontrarSolicitud(idSolicitud);
        if (!solicitud) {
            std::cout << "✗ Solicitud no encontrada.\n";
            return false;
        }

        // Estado y asignación previa
        if (solicitud->getIdMensajero() != NO_MENSAJERO) {
            std::cout << "✗ La solicitud ya tiene un mensajero asignado (ID "
                      << solicitud->getIdMensajero() << ").\n";
            return false;
        }
        if (solicitud->getEstado() != EstadoPaquete::SOLICITADO) {
            std::cout << "✗ La solicitud no está en estado SOLICITADO.\n";
            return false;
        }

        // Verificar mensajero
        Usuario* mensajero = encontrarMensajeroActivo(idMensajero);
        if (!mensajero) {
            std::cout << "✗ Mensajero no válido o inactivo.\n";
            return false;
        }

        // Asignar mensajero
        solicitud->asignarMensajero(idMensajero);
        
        // solicitud->setEstado(EstadoPaquete::EN_RECOLECCION);

        // Crear mensaje de notificación
        std::string contenido = "Nueva recolección asignada.\n";
        contenido += "Solicitud #" + std::to_string(idSolicitud) + "\n";

        // Evitar null en paquete
        if (solicitud->getPaquete()) {
            contenido += "Origen: "  + solicitud->getPaquete()->getDirOrigen()  + "\n";
            contenido += "Destino: " + solicitud->getPaquete()->getDirDestino();
        } else {
            contenido += "(Detalles del paquete no disponibles)";
        }

        Mensaje* mensaje = new Mensaje(siguienteIdMensaje++,
                                       idControlador,
                                       idMensajero,
                                       idSolicitud,
                                       contenido);
        mensajes.push_back(mensaje);

        std::cout << "\n✓ Mensajero asignado exitosamente.\n";
        std::cout << "Mensajero " << mensajero->getNombre()
                  << " ha sido notificado.\n";
        return true;
    }

    // Ver solicitudes sin asignar
    int verSolicitudesSinAsignar() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║   SOLICITUDES SIN MENSAJERO            ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n\n";

        int cont = 0;
        for (const auto* solicitud : solicitudes) {
            if (!solicitud) continue;
            if (solicitud->getEstado() == EstadoPaquete::SOLICITADO &&
                solicitud->getIdMensajero() == NO_MENSAJERO) {
                solicitud->mostrarResumen();
                std::cout << '\n';
                ++cont;
            }
        }
        if (cont == 0) {
            std::cout << "No hay solicitudes sin asignar.\n";
        }
        return cont;
    }

    // Ver solicitudes en proceso
    int verSolicitudesEnProceso() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║     SOLICITUDES EN PROCESO             ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n\n";

        int cont = 0;
        for (const auto* solicitud : solicitudes) {
            if (!solicitud) continue;
            auto st = solicitud->getEstado();
            if (st == EstadoPaquete::EN_RECOLECCION ||
                st == EstadoPaquete::EN_TRANSITO) {
                solicitud->mostrarResumen();
                std::cout << "  Mensajero ID: " << solicitud->getIdMensajero() << "\n\n";
                ++cont;
            }
        }
        if (cont == 0) {
            std::cout << "No hay solicitudes en proceso.\n";
        }
        return cont;
    }

    // Enviar mensaje personalizado
    bool enviarMensaje(int idRemitente, int idDestinatario,
                       int idSolicitud, const std::string& contenido)
    {
        
        Usuario* remitente = encontrarUsuario(idRemitente);
        Usuario* destinatario = encontrarUsuario(idDestinatario);

        if (!remitente || !remitente->estaActivo()) {
            std::cout << "✗ Remitente no válido o inactivo.\n";
            return false;
        }
        if (!destinatario) {
            std::cout << "✗ Destinatario no encontrado.\n";
            return false;
        }

        // Valida que la solicitud existe cuando se pasa un id > 0
        if (idSolicitud > 0 && !encontrarSolicitud(idSolicitud)) {
            std::cout << "✗ Solicitud asociada no encontrada.\n";
            return false;
        }

        Mensaje* mensaje = new Mensaje(siguienteIdMensaje++,
                                       idRemitente,
                                       idDestinatario,
                                       idSolicitud,
                                       contenido);
        mensajes.push_back(mensaje);

        std::cout << "✓ Mensaje enviado exitosamente.\n";
        return true;
    }
};

#endif 
// CONTROLADOR_CONTROLLER_H
