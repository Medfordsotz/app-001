#ifndef VISTA_MENSAJERO_H
#define VISTA_MENSAJERO_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "../models/Mensaje.h"
#include "../models/Solicitud.h"
#include "../models/Enums.h"
#include "../controllers/SolicitudController.h"
#include "../utils/Utilidades.h"

class VistaMensajero {
private:
    std::vector<Mensaje*>& mensajes;
    SolicitudController& solicitudController;

    static void limpiarEntrada() {
        if (!std::cin.good()) std::cin.clear();
        Validaciones::limpiarBuffer();
    }

    // Reúne solicitudes por estado y filtra por idMensajero
    std::vector<Solicitud*> asignacionesDe(int idMensajero, bool soloActivas) {
        std::vector<Solicitud*> out;

        auto agrega = [&](EstadoPaquete est) {
            auto v = solicitudController.obtenerPorEstado(est);
            for (auto* s : v) {
                if (!s) continue;
                if (s->getIdMensajero() == idMensajero) {
                    if (!soloActivas || (est != EstadoPaquete::ENTREGADO && est != EstadoPaquete::CANCELADO)) {
                        out.push_back(s);
                    }
                }
            }
        };

        // Traemos todos los estados y luego filtramos
        agrega(EstadoPaquete::SOLICITADO);
        agrega(EstadoPaquete::EN_RECOLECCION);
        agrega(EstadoPaquete::EN_TRANSITO);
        agrega(EstadoPaquete::ENTREGADO);
        agrega(EstadoPaquete::CANCELADO);

        return out;
    }

public:
    VistaMensajero(std::vector<Mensaje*>& msgs, SolicitudController& solCtrl)
        : mensajes(msgs), solicitudController(solCtrl) {}

    void mostrarMenu(const std::string& nombreUsuario) {
        UI::limpiarPantalla();
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     MENÚ MENSAJERO                     ║\n";
        std::cout << "║     Usuario: " << nombreUsuario << "\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n1. Ver mis mensajes\n";
        std::cout << "2. Mis asignaciones\n";
        std::cout << "3. Actualizar estado de paquete\n";
        std::cout << "4. Ver detalle de solicitud\n";
        std::cout << "5. Cerrar sesión\n";
        std::cout << "\nSeleccione una opción: ";
    }

    void verMensajes(int idUsuario) {
        UI::limpiarPantalla();
        UI::mostrarTitulo("MIS MENSAJES");

        bool tieneMensajes = false;
        for (auto* mensaje : mensajes) {
            if (mensaje && mensaje->getIdDestinatario() == idUsuario) {
                mensaje->mostrar();
                mensaje->marcarLeido();
                std::cout << "\n";
                tieneMensajes = true;
            }
        }

        if (!tieneMensajes) {
            std::cout << "No tiene mensajes.\n";
        }
        UI::pausa();
    }

    void verMisAsignaciones(int idMensajero) {
        UI::limpiarPantalla();
        UI::mostrarTitulo("MIS ASIGNACIONES");

        auto asign = asignacionesDe(idMensajero, /*soloActivas=*/true);
        if (asign.empty()) {
            std::cout << "No tiene asignaciones pendientes.\n";
            UI::pausa();
            return;
        }

        for (auto* s : asign) {
            if (s) { s->mostrarResumen(); std::cout << "\n"; }
        }
        UI::pausa();
    }

    void actualizarEstadoPaquete(int idMensajero) {
        UI::limpiarPantalla();
        UI::mostrarTitulo("ACTUALIZAR ESTADO DE PAQUETE");

        // Mostrar asignaciones activas
        std::cout << "Sus asignaciones activas:\n\n";
        auto asign = asignacionesDe(idMensajero, /*soloActivas=*/true);
        if (asign.empty()) {
            std::cout << "No tiene asignaciones para actualizar.\n";
            UI::pausa();
            return;
        }
        for (auto* s : asign) { if (s) { s->mostrarResumen(); std::cout << "\n"; } }

        int idSolicitud = 0;
        std::cout << "ID de la solicitud: ";
        if (!(std::cin >> idSolicitud)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        // Verificar asignación
        Solicitud* solicitud = solicitudController.buscarPorId(idSolicitud);
        if (!solicitud || solicitud->getIdMensajero() != idMensajero) {
            std::cout << "✗ Esta solicitud no está asignada a usted.\n";
            UI::pausa();
            return;
        }

        std::cout << "\n--- ESTADOS DISPONIBLES ---\n";
        std::cout << "1. En Recolección\n";
        std::cout << "2. En Tránsito\n";
        std::cout << "3. Entregado\n";
        std::cout << "Seleccione nuevo estado: ";

        int nuevoEstado = 0;
        if (!(std::cin >> nuevoEstado)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        EstadoPaquete est;
        switch (nuevoEstado) {
            case 1: est = EstadoPaquete::EN_RECOLECCION; break;
            case 2: est = EstadoPaquete::EN_TRANSITO;    break;
            case 3: est = EstadoPaquete::ENTREGADO;      break;
            default:
                std::cout << "✗ Estado inválido.\n";
                UI::pausa();
                return;
        }

        if (solicitudController.actualizarEstado(idSolicitud, est)) {
            std::cout << "\n✓ Estado actualizado exitosamente.\n";
            if (est == EstadoPaquete::ENTREGADO) {
                std::cout << "¡Entrega completada!\n";
            }
        } else {
            std::cout << "\n✗ Error al actualizar el estado.\n";
        }
        UI::pausa();
    }

    void verDetalleSolicitud() {
        UI::limpiarPantalla();
        UI::mostrarTitulo("DETALLE DE SOLICITUD");

        int id = 0;
        std::cout << "Ingrese el ID de la solicitud: ";
        if (!(std::cin >> id)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        Solicitud* s = solicitudController.buscarPorId(id);
        if (!s) {
            std::cout << "No se encontró la solicitud.\n";
        } else {
            s->mostrarInfo();
        }
        UI::pausa();
    }
};

#endif // VISTA_MENSAJERO_H
