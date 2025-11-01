#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <windows.h>  


// Modelos
#include "models/Enums.h"
#include "models/Usuario.h"
#include "models/Paquete.h"
#include "models/Solicitud.h"
#include "models/Mensaje.h"

// Utilidades
#include "utils/Utilidades.h"
#include "utils/CalculoTarifas.h"

// Controladores
#include "controllers/AuthController.h"
#include "controllers/AdminController.h"
#include "controllers/SolicitudController.h"
#include "controllers/ControladorController.h"

// Vistas
#include "views/MenuPrincipal.h"
#include "views/VistaCliente.h"
#include "views/VistaAdmin.h"
#include "views/VistaControlador.h"
#include "views/VistaMensajero.h"

using namespace std;

static void limpiarEntrada() {
    if (!cin.good()) cin.clear();
    Validaciones::limpiarBuffer();
}

int main() {
    // Contenedores de datos

      SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("cls");

    vector<Usuario*> usuarios;
    vector<Paquete*> paquetes;
    vector<Solicitud*> solicitudes;
    vector<Mensaje*> mensajes;

    // IDs autoincrementables
    int siguienteIdUsuario   = 1;
    int siguienteIdPaquete   = 1;
    int siguienteIdSolicitud = 1;
    int siguienteIdMensaje   = 1;

    // Usuarios base
    Usuario* admin = new Usuario(siguienteIdUsuario++, "Administrador",
                                 "admin@envios.com", "admin123",
                                 Rol::ADMINISTRADOR);
    usuarios.push_back(admin);

    Usuario* cliente1 = new Usuario(siguienteIdUsuario++, "Juan Pérez",
                                    "juan@email.com", "juan123", Rol::CLIENTE);
    usuarios.push_back(cliente1);

    Usuario* controlador1 = new Usuario(siguienteIdUsuario++, "María López",
                                        "maria@envios.com", "maria123",
                                        Rol::CONTROLADOR);
    usuarios.push_back(controlador1);

    Usuario* mensajero1 = new Usuario(siguienteIdUsuario++, "Carlos García",
                                      "carlos@envios.com", "carlos123",
                                      Rol::MENSAJERO);
    usuarios.push_back(mensajero1);

    // Controladores
    AuthController authController(usuarios, siguienteIdUsuario);
    SolicitudController solicitudController(paquetes, solicitudes,
                                            siguienteIdPaquete, siguienteIdSolicitud);
    AdminController adminController(usuarios, solicitudes);
    ControladorController controladorController(solicitudes, mensajes,
                                                usuarios, siguienteIdMensaje);

    // Vistas
    MenuPrincipal   menuPrincipal(authController);
    VistaCliente    vistaCliente(solicitudController, mensajes);
    VistaAdmin      vistaAdmin(adminController, authController);
    VistaControlador vistaControlador(controladorController, solicitudController);
    VistaMensajero  vistaMensajero(mensajes, solicitudController);

    // Bucle principal
    int opcionPrincipal = -1;
    bool salir = false;

    do {
        menuPrincipal.mostrar();
        if (!(cin >> opcionPrincipal)) { cout << "Entrada inválida.\n"; limpiarEntrada(); continue; }
        limpiarEntrada();

        switch (opcionPrincipal) {
            case 1: { // Iniciar sesión
                string email, password;
                menuPrincipal.mostrarLogin();
                // Leemos email y password con getline para evitar problemas de salto de línea
                getline(cin, email);
                cout << "Contraseña: ";
                getline(cin, password);

                if (menuPrincipal.procesarLogin(email, password)) {
                    Usuario* usuarioActual = authController.getUsuarioActual();
                    if (!usuarioActual) { cout << "Error inesperado de sesión.\n"; break; }

                    bool volverAlMenuPrincipal = false;

                    switch (usuarioActual->getRol()) {
                        case Rol::CLIENTE: {
                            int opcion = 0;
                            do {
                                vistaCliente.mostrarMenu(usuarioActual->getNombre());
                                if (!(cin >> opcion)) { cout << "Entrada inválida.\n"; limpiarEntrada(); continue; }
                                limpiarEntrada();

                                switch (opcion) {
                                    case 1: vistaCliente.solicitarEnvio(usuarioActual->getId()); break;
                                    case 2: vistaCliente.verMisSolicitudes(usuarioActual->getId()); break;
                                    case 3: vistaCliente.verDetalleSolicitud(); break;
                                    case 4: vistaCliente.realizarPago(usuarioActual->getId()); break;
                                    case 5: vistaCliente.verMensajes(usuarioActual->getId()); break;
                                    case 6: authController.cerrarSesion(); volverAlMenuPrincipal = true; break;
                                    default: cout << "Opción inválida.\n"; break;
                                }
                            } while (!volverAlMenuPrincipal && authController.hayUsuarioAutenticado());
                            break;
                        }

                        case Rol::ADMINISTRADOR: {
                            int opcion = 0;
                            do {
                                vistaAdmin.mostrarMenu(usuarioActual->getNombre());
                                if (!(cin >> opcion)) { cout << "Entrada inválida.\n"; limpiarEntrada(); continue; }
                                limpiarEntrada();

                                switch (opcion) {
                                    case 1:  vistaAdmin.listarUsuarios(); break;
                                    case 2:  vistaAdmin.verDetalleUsuario(); break;
                                    case 3:  vistaAdmin.registrarUsuario(); break;
                                    case 4:  vistaAdmin.activarUsuario(); break;
                                    case 5:  vistaAdmin.desactivarUsuario(); break;
                                    case 6:  vistaAdmin.eliminarUsuario(); break;
                                    case 7:  vistaAdmin.cambiarRolUsuario(); break;
                                    case 8:  vistaAdmin.verTarifas(); break;
                                    case 9:  vistaAdmin.modificarTarifaBase(); break;
                                    case 10: vistaAdmin.modificarMultiplicadorNocturno(); break;
                                    case 11: vistaAdmin.verTodasLasSolicitudes(); break;
                                    case 12: vistaAdmin.verReporteGeneral(); break;
                                    case 13: authController.cerrarSesion(); volverAlMenuPrincipal = true; break;
                                    default: cout << "Opción inválida.\n"; break;
                                }
                            } while (!volverAlMenuPrincipal && authController.hayUsuarioAutenticado());
                            break;
                        }

                        case Rol::CONTROLADOR: {
                            int opcion = 0;
                            do {
                                vistaControlador.mostrarMenu(usuarioActual->getNombre());
                                if (!(cin >> opcion)) { cout << "Entrada inválida.\n"; limpiarEntrada(); continue; }
                                limpiarEntrada();

                                switch (opcion) {
                                    case 1: vistaControlador.verSolicitudesSinAsignar(); break;
                                    case 2: vistaControlador.verSolicitudesEnProceso(); break;
                                    case 3: vistaControlador.asignarMensajero(usuarioActual->getId()); break;
                                    case 4: vistaControlador.verMensajeros(); break;
                                    case 5: vistaControlador.verTodasSolicitudes(); break;
                                    case 6: vistaControlador.enviarMensaje(usuarioActual->getId()); break;
                                    case 7: authController.cerrarSesion(); volverAlMenuPrincipal = true; break;
                                    default: cout << "Opción inválida.\n"; break;
                                }
                            } while (!volverAlMenuPrincipal && authController.hayUsuarioAutenticado());
                            break;
                        }

                        case Rol::MENSAJERO: {
                            int opcion = 0;
                            do {
                                vistaMensajero.mostrarMenu(usuarioActual->getNombre());
                                if (!(cin >> opcion)) { cout << "Entrada inválida.\n"; limpiarEntrada(); continue; }
                                limpiarEntrada();

                                switch (opcion) {
                                    case 1: vistaMensajero.verMensajes(usuarioActual->getId()); break;
                                    case 2: vistaMensajero.verMisAsignaciones(usuarioActual->getId()); break;
                                    case 3: vistaMensajero.actualizarEstadoPaquete(usuarioActual->getId()); break;
                                    case 4: vistaMensajero.verDetalleSolicitud(); break;
                                    case 5: authController.cerrarSesion(); volverAlMenuPrincipal = true; break;
                                    default: cout << "Opción inválida.\n"; break;
                                }
                            } while (!volverAlMenuPrincipal && authController.hayUsuarioAutenticado());
                            break;
                        }
                    } // fin switch rol
                } else {
                    cout << "\n✗ Credenciales inválidas o usuario inactivo.\n";
                    UI::pausa();
                }
                break;
            }

            case 2: { // Registrarse (opcional: usa MenuPrincipal para registro básico)
                menuPrincipal.mostrarRegistro();
                menuPrincipal.procesarRegistroInteractivo();
                break;
            }

            case 0:
                salir = true;
                break;

            default:
                cout << "Opción inválida.\n";
                break;
        }
    } while (!salir);

    // --- Liberar memoria ---
    for (auto* u : usuarios)     delete u;
    for (auto* p : paquetes)     delete p;
    for (auto* s : solicitudes)  delete s;
    for (auto* m : mensajes)     delete m;

    return 0;
}
