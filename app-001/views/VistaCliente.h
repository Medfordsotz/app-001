#ifndef VISTA_CLIENTE_H
#define VISTA_CLIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "../controllers/SolicitudController.h"
#include "../models/Mensaje.h"
#include "../models/Enums.h"
#include "../utils/Utilidades.h"

class VistaCliente {
private:
    SolicitudController& solicitudController;
    std::vector<Mensaje*>& mensajes;

    static void limpiarEntrada() {
        if (!std::cin.good()) std::cin.clear();
        Validaciones::limpiarBuffer();
    }

public:
    VistaCliente(SolicitudController& solCtrl, std::vector<Mensaje*>& msgs)
        : solicitudController(solCtrl), mensajes(msgs) {}

    void mostrarMenu(const std::string& nombreUsuario) {
        UI::limpiarPantalla();
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     MENÚ CLIENTE                       ║\n";
        std::cout << "║     Usuario: " << nombreUsuario << "\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n1. Solicitar nuevo envío\n";
        std::cout << "2. Mis solicitudes\n";
        std::cout << "3. Ver detalle de solicitud\n";
        std::cout << "4. Realizar pago\n";
        std::cout << "5. Ver mensajes\n";
        std::cout << "6. Cerrar sesión\n";
        std::cout << "\nSeleccione una opción: ";
    }

   void solicitarEnvio(int idUsuario) {
    UI::limpiarPantalla();
    UI::mostrarTitulo("NUEVA SOLICITUD DE ENVÍO");
    std::cout << "Ingrese 0 en cualquier campo para regresar al menú.\n\n";

    float peso = 0.0f;
    std::string descripcion, origen, destino;
    std::string nombreDest, telDest;

    std::cout << "Peso del paquete (máx " << PESO_MAXIMO << " lbs): ";
    if (!(std::cin >> peso)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (peso == 0.0f) return; // ← regresar al menú

    if (!Validaciones::validarPeso(peso)) {
        std::cout << "Peso inválido.\n";
        UI::pausa();
        return;
    }

    std::cout << "Descripción del contenido: ";
    std::getline(std::cin, descripcion);
    if (descripcion == "0") return;

    std::cout << "\n--- INFORMACIÓN DE ORIGEN ---\n";
    std::cout << "Dirección de recolección: ";
    std::getline(std::cin, origen);
    if (origen == "0") return;

    std::cout << "\n--- INFORMACIÓN DE DESTINO ---\n";
    std::cout << "Dirección de entrega: ";
    std::getline(std::cin, destino);
    if (destino == "0") return;

    std::cout << "Nombre del destinatario: ";
    std::getline(std::cin, nombreDest);
    if (nombreDest == "0") return;

    std::cout << "Teléfono del destinatario: ";
    std::getline(std::cin, telDest);
    if (telDest == "0") return;

    // Crear Paquete + Solicitud
    Paquete* paq = solicitudController.crearPaquete(peso, descripcion, origen, destino);
    if (!paq) {
        std::cout << "No se pudo crear el paquete (verifique el peso/datos).\n";
        UI::pausa();
        return;
    }

    paq->setNombreDestinatario(nombreDest);
    paq->setTelefonoDestinatario(telDest);

    Solicitud* sol = solicitudController.crearSolicitud(idUsuario, paq);
    if (!sol) {
        std::cout << "No se pudo crear la solicitud.\n";
        UI::pausa();
        return;
    }

    std::cout << "\n✓ Solicitud creada exitosamente.\n";
    std::cout << "ID: " << sol->getId()
              << " | Monto: Q" << std::fixed << std::setprecision(2)
              << sol->getMontoTotal() << "\n";
    UI::pausa();
}


    void verMisSolicitudes(int idUsuario) {
        UI::limpiarPantalla();
        UI::mostrarTitulo("MIS SOLICITUDES");

        auto mias = solicitudController.obtenerPorUsuario(idUsuario);
        if (mias.empty()) {
            std::cout << "No tienes solicitudes.\n";
            UI::pausa();
            return;
        }
        for (auto* s : mias) {
            if (s) s->mostrarResumen();
            std::cout << "\n";
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

    void realizarPago(int idUsuario) {
        UI::limpiarPantalla();
        UI::mostrarTitulo("PROCESAR PAGO");

        auto mias = solicitudController.obtenerPorUsuario(idUsuario);
        bool hayPendientes = false;
        for (auto* s : mias) {
            if (s && !s->estaPagado()) {
                s->mostrarResumen();
                std::cout << "\n";
                hayPendientes = true;
            }
        }

        if (!hayPendientes) {
            std::cout << "No tiene pagos pendientes.\n";
            UI::pausa();
            return;
        }

        int idSolicitud = 0;
        std::cout << "Ingrese el ID de la solicitud a pagar: ";
        if (!(std::cin >> idSolicitud)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        std::cout << "\n--- MÉTODOS DE PAGO ---\n";
        std::cout << "1. Tarjeta de crédito\n";
        std::cout << "2. Tarjeta de débito\n";
        std::cout << "3. Transferencia bancaria\n";
        std::cout << "Seleccione método: ";

        int metodo = 0;
        if (!(std::cin >> metodo)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        if (metodo < 1 || metodo > 3) {
            std::cout << "✗ Método de pago inválido.\n";
            UI::pausa();
            return;
        }

        Solicitud* s = solicitudController.buscarPorId(idSolicitud);
        if (!s) {
            std::cout << "No se encontró la solicitud.\n";
            UI::pausa();
            return;
        }
        if (s->estaPagado()) {
            std::cout << "Esta solicitud ya está pagada.\n";
            UI::pausa();
            return;
        }

        // Aquí iría la integración con pasarela; por ahora, marcamos pagado.
        s->marcarPagado();
        std::cout << "✓ Pago registrado. Gracias.\n";
        UI::pausa();
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
};

#endif // VISTA_CLIENTE_H
