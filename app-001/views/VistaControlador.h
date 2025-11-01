#ifndef VISTA_CONTROLADOR_H
#define VISTA_CONTROLADOR_H

#include <iostream>
#include <vector>
#include <iomanip>
#include "../controllers/ControladorController.h"
#include "../controllers/SolicitudController.h"
#include "../models/Enums.h"
#include "../utils/Utilidades.h"

class VistaControlador {
private:
    ControladorController& controladorController;
    SolicitudController& solicitudController;

    static void limpiarEntrada() {
        if (!std::cin.good()) std::cin.clear();
        Validaciones::limpiarBuffer();
    }

public:
    VistaControlador(ControladorController& ctrlCtrl, SolicitudController& solCtrl)
        : controladorController(ctrlCtrl), solicitudController(solCtrl) {}

    void mostrarMenu(const std::string& nombreUsuario) {
        UI::limpiarPantalla();
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     MENÚ CONTROLADOR                   ║\n";
        std::cout << "║     Usuario: " << nombreUsuario << "\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n1. Ver solicitudes sin asignar\n";
        std::cout << "2. Ver solicitudes en proceso\n";
        std::cout << "3. Asignar mensajero a solicitud\n";
        std::cout << "4. Ver mensajeros disponibles\n";
        std::cout << "5. Ver todas las solicitudes\n";
        std::cout << "6. Enviar mensaje\n";
        std::cout << "7. Cerrar sesión\n";
        std::cout << "\nSeleccione una opción: ";
    }

    void verSolicitudesSinAsignar() {
        UI::limpiarPantalla();
        controladorController.verSolicitudesSinAsignar();
        UI::pausa();
    }

    void verSolicitudesEnProceso() {
        UI::limpiarPantalla();
        controladorController.verSolicitudesEnProceso();
        UI::pausa();
    }

    void asignarMensajero(int idControlador) {
    UI::limpiarPantalla();
    UI::mostrarTitulo("ASIGNAR MENSAJERO");
    std::cout << "Ingrese 0 en cualquier campo para regresar al menú.\n\n";

    // Mostrar contexto
    std::cout << "— Solicitudes sin asignar —\n";
    controladorController.verSolicitudesSinAsignar();
    std::cout << "\n— Mensajeros disponibles —\n";
    controladorController.listarMensajeros();

    int idSolicitud = 0, idMensajero = 0;

    std::cout << "\nID de la solicitud: ";
    if (!(std::cin >> idSolicitud)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    if (idSolicitud == 0) return; // ← opción para regresar
     
    std::cout << "ID del mensajero: ";
    if (!(std::cin >> idMensajero)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (idMensajero == 0) return; // ← opción para regresar

    controladorController.asignarMensajero(idSolicitud, idMensajero, idControlador);
    UI::pausa();
}


    void verMensajeros() {
        UI::limpiarPantalla();
        controladorController.listarMensajeros();
        UI::pausa();
    }

    
    // se listan por estado usando SolicitudController::obtenerPorEstado
    void verTodasSolicitudes() {
        UI::limpiarPantalla();
        UI::mostrarTitulo("TODAS LAS SOLICITUDES");

        auto listarPor = [&](EstadoPaquete est, const char* titulo) {
            auto vec = solicitudController.obtenerPorEstado(est);
            if (!vec.empty()) {
                std::cout << "\n--- " << titulo << " (" << vec.size() << ") ---\n";
                for (auto* s : vec) { if (s) s->mostrarResumen(); }
            }
        };

        listarPor(EstadoPaquete::SOLICITADO,     "SOLICITADO");
        listarPor(EstadoPaquete::EN_RECOLECCION, "EN RECOLECCIÓN");
        listarPor(EstadoPaquete::EN_TRANSITO,    "EN TRÁNSITO");
        listarPor(EstadoPaquete::ENTREGADO,      "ENTREGADO");
        listarPor(EstadoPaquete::CANCELADO,      "CANCELADO");

        UI::pausa();
    }

    void enviarMensaje(int idRemitente) {
    UI::limpiarPantalla();
    UI::mostrarTitulo("ENVIAR MENSAJE");
    std::cout << "0 = regresar | Dejar vacío = no aplica (solo en solicitud)\n\n";

    std::string linea;
    int idDestinatario = 0;
    int idSolicitud    = 0;
    std::string contenido;

    // ---- Destinatario ----
    std::cout << "ID del destinatario (usuario) [0=regresar]: ";
    std::getline(std::cin, linea);
    if (linea == "0") return;
    try {
        idDestinatario = std::stoi(linea);
    } catch (...) {
        std::cout << "Entrada inválida (debe ser un número de usuario).\n";
        UI::pausa();
        return;
    }

    // ---- Solicitud (opcional) ----
    std::cout << "ID de la solicitud relacionada [vacío=no aplica, 0=regresar]: ";
    std::getline(std::cin, linea);
    if (linea == "0") return;
    if (!linea.empty()) {
        try {
            idSolicitud = std::stoi(linea); 
        } catch (...) {
            std::cout << "Entrada inválida (debe ser un número o vacío).\n";
            UI::pausa();
            return;
        }
    } else {
        idSolicitud = 0; 
    }

    // ---- Mensaje ----
    std::cout << "Mensaje [0=regresar]: ";
    std::getline(std::cin, contenido);
    if (contenido == "0") return;

    controladorController.enviarMensaje(idRemitente, idDestinatario, idSolicitud, contenido);
    std::cout << "\n✓ Mensaje enviado exitosamente.\n";
    UI::pausa();
}

};

#endif // VISTA_CONTROLADOR_H
