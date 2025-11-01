#ifndef VISTA_ADMIN_H
#define VISTA_ADMIN_H

#include <iostream>
#include <limits>
#include "../controllers/AdminController.h"
#include "../controllers/AuthController.h"
#include "../utils/Utilidades.h"
#include "../utils/CalculoTarifas.h"
#include "../models/Enums.h"

class VistaAdmin {
private:
    AdminController& adminController;
    AuthController& authController;

    static void limpiarEntrada() {
        if (!std::cin.good()) std::cin.clear();
        Validaciones::limpiarBuffer();
    }

public:
    VistaAdmin(AdminController& admin, AuthController& auth)
        : adminController(admin), authController(auth) {}

    void mostrarMenu(const std::string& nombreUsuario) {
        UI::limpiarPantalla();
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║     MENÚ ADMINISTRADOR                 ║\n";
        std::cout << "║     Usuario: " << nombreUsuario << "\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n=== GESTIÓN DE USUARIOS ===\n";
        std::cout << "1. Listar usuarios\n";
        std::cout << "2. Ver detalle de usuario\n";
        std::cout << "3. Registrar nuevo usuario\n";
        std::cout << "4. Activar usuario\n";
        std::cout << "5. Desactivar usuario\n";
        std::cout << "6. Eliminar usuario\n";
        std::cout << "7. Cambiar rol de usuario\n";
        std::cout << "\n=== GESTIÓN DE TARIFAS ===\n";
        std::cout << "8. Ver tarifas actuales\n";
        std::cout << "9. Modificar tarifa base\n";
        std::cout << "10. Modificar multiplicador nocturno\n";
        std::cout << "\n=== SOLICITUDES Y REPORTES ===\n";
        std::cout << "11. Ver todas las solicitudes\n";
        std::cout << "12. Reporte general\n";
        std::cout << "\n13. Cerrar sesión\n";
        std::cout << "\nSeleccione una opción: ";
    }

    // --- Usuarios ---
    void listarUsuarios() {
        UI::limpiarPantalla();
        adminController.listarUsuarios();
        UI::pausa();
    }

    void verDetalleUsuario() {
        UI::limpiarPantalla();
        UI::mostrarTitulo("DETALLE DE USUARIO");

        int id = 0;
        std::cout << "Ingrese el ID del usuario: ";
        if (!(std::cin >> id)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        Usuario* u = adminController.buscarUsuarioPorId(id);
        if (!u) {
            std::cout << "✗ Usuario no encontrado.\n";
        } else {
            u->mostrarInfo();
        }
        UI::pausa();
    }

    void registrarUsuario() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("REGISTRAR NUEVO USUARIO");
    std::cout << "Escriba 0 en cualquier campo para regresar al menú.\n\n";

    std::string nombre, email, password;
    int rolNum;

    std::cout << "Nombre completo: ";
    std::getline(std::cin, nombre);
    if (nombre == "0") return;

    std::cout << "Email: ";
    std::getline(std::cin, email);
    if (email == "0") return;

    std::cout << "Contraseña: ";
    std::getline(std::cin, password);
    if (password == "0") return;

    std::cout << "\n--- ROLES DISPONIBLES ---\n";
    std::cout << "0. Cliente\n";
    std::cout << "1. Administrador\n";
    std::cout << "2. Controlador\n";
    std::cout << "3. Mensajero\n";
    std::cout << "9. Cancelar / Regresar\n";
    std::cout << "Seleccione rol: ";

    if (!(std::cin >> rolNum)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (rolNum == 9) return;                    // ← salir sin registrar
    if (rolNum < 0 || rolNum > 3) {
        std::cout << "✗ Rol inválido.\n";
        UI::pausa();
        return;
    }

    bool ok = authController.registrarUsuario(
        nombre, email, password, static_cast<Rol>(rolNum)
    );

    if (ok) std::cout << "\n✓ Usuario registrado exitosamente.\n";
    else    std::cout << "\n✗ Error al registrar usuario (email inválido o duplicado).\n";

    UI::pausa();
}

 void activarUsuario() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("ACTIVAR USUARIO");
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    int id = 0;
    std::cout << "Ingrese el ID del usuario a activar: ";
    if (!(std::cin >> id)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (id == 0) return; // ← opción para regresar

    Usuario* u = adminController.buscarUsuarioPorId(id);
    if (!u) {
        std::cout << "✗ Usuario no encontrado.\n";
    } else {
        u->activar();
        std::cout << "✓ Usuario activado exitosamente.\n";
    }
    UI::pausa();
}

void desactivarUsuario() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("DESACTIVAR USUARIO");
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    int id = 0;
    std::cout << "Ingrese el ID del usuario a desactivar: ";
    if (!(std::cin >> id)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (id == 0) return; // ← opción para regresar

    Usuario* u = adminController.buscarUsuarioPorId(id);
    if (!u) {
        std::cout << "✗ Usuario no encontrado.\n";
    } else {
        u->desactivar();
        std::cout << "✓ Usuario desactivado exitosamente.\n";
    }
    UI::pausa();
}

void eliminarUsuario() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("ELIMINAR USUARIO");
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    int id = 0;
    std::cout << "Ingrese el ID del usuario a eliminar: ";
    if (!(std::cin >> id)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (id == 0) return; 
    // ← permite regresar sin eliminar

    adminController.eliminarUsuarioPorId(id); 
    UI::pausa();
}


    void cambiarRolUsuario() {
        UI::limpiarPantalla();
        UI::mostrarTitulo("CAMBIAR ROL DE USUARIO");
        std::cout << "Ingrese 0 para regresar al menú.\n\n";

        int id = 0;
        std::cout << "ID del usuario: ";
        if (!(std::cin >> id)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();
         if (id == 0) return;

        std::cout << "\n--- ROLES DISPONIBLES ---\n";
        std::cout << "0. Cliente\n";
        std::cout << "1. Administrador\n";
        std::cout << "2. Controlador\n";
        std::cout << "3. Mensajero\n";
        std::cout << "Seleccione nuevo rol: ";

        int rolNum = 0;
        if (!(std::cin >> rolNum)) {
            std::cout << "Entrada inválida.\n";
            limpiarEntrada();
            UI::pausa();
            return;
        }
        limpiarEntrada();

        if (rolNum < 0 || rolNum > 3) {
            std::cout << "✗ Rol inválido.\n";
            UI::pausa();
            return;
        }

        if (adminController.cambiarRolUsuario(id, static_cast<Rol>(rolNum))) {
            std::cout << "✓ Rol actualizado.\n";
        }
        UI::pausa();
    }

    // --- Tarifas ---
    void verTarifas() {
        UI::limpiarPantalla();
        UI::mostrarTitulo("TARIFAS ACTUALES");
        std::cout << "Tarifa base: Q" << CalculadoraTarifas::getTarifaBase() << "\n";
        std::cout << "Multiplicador nocturno: x" << CalculadoraTarifas::getMultiplicadorNocturno() << "\n";
        std::cout << "Horario diurno: " << HORA_INICIO_DIURNO << ":00 - " << HORA_FIN_DIURNO << ":00\n";
        UI::pausa();
    }

void modificarTarifaBase() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("MODIFICAR TARIFA BASE");
    std::cout << "Actual: Q" << CalculadoraTarifas::getTarifaBase() << "\n";
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    float nueva = 0.0f;
    std::cout << "Nueva tarifa base (Q): ";
    if (!(std::cin >> nueva)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (nueva == 0.0f) return; // ← opción para regresar sin modificar

    CalculadoraTarifas::setTarifaBase(nueva);
    std::cout << "✓ Tarifa base actualizada a Q" << CalculadoraTarifas::getTarifaBase() << "\n";
    UI::pausa();
}


   void modificarMultiplicadorNocturno() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("MODIFICAR MULTIPLICADOR NOCTURNO");
    std::cout << "Actual: x" << CalculadoraTarifas::getMultiplicadorNocturno() << "\n";
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    float mult = 0.0f;
    std::cout << "Nuevo multiplicador: x";
    if (!(std::cin >> mult)) {
        std::cout << "Entrada inválida.\n";
        limpiarEntrada();
        UI::pausa();
        return;
    }
    limpiarEntrada();

    if (mult == 0.0f) return; // ← opción para regresar sin modificar

    CalculadoraTarifas::setMultiplicadorNocturno(mult);
    std::cout << "✓ Multiplicador actualizado a x" 
              << CalculadoraTarifas::getMultiplicadorNocturno() << "\n";
    UI::pausa();
}


   void verTodasLasSolicitudes() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("LISTADO DE TODAS LAS SOLICITUDES");
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    std::string opcion;
    std::cout << "Presione Enter para ver las solicitudes o 0 para regresar: ";
    std::getline(std::cin, opcion);

    if (opcion == "0") return; // ← opción para regresar sin mostrar

    UI::limpiarPantalla();
    adminController.listarSolicitudes();

    std::cout << "\n";
    UI::pausa();
}
//Reporte General//

   void verReporteGeneral() {
    UI::limpiarPantalla();
    UI::mostrarTitulo("REPORTE GENERAL");
    std::cout << "Ingrese 0 para regresar al menú.\n\n";

    std::string opcion;
    std::cout << "Presione Enter para generar el reporte o 0 para regresar: ";
    std::getline(std::cin, opcion);

    if (opcion == "0") return; // ← opción para regresar sin ejecutar reporte

    UI::limpiarPantalla();
    adminController.reporteResumen();

    std::cout << "\n";
    UI::pausa();
}

};

#endif
// VISTA_ADMIN_H
