#ifndef MENU_PRINCIPAL_H
#define MENU_PRINCIPAL_H

#include <iostream>
#include <string>
#include "../controllers/AuthController.h"
#include "../models/Enums.h"
#include "../utils/Utilidades.h"
#include <windows.h>

class MenuPrincipal {
private:
    AuthController& auth;

    static void limpiarEntrada() {
        if (!std::cin.good()) std::cin.clear();
        Validaciones::limpiarBuffer();
    }

public:
    explicit MenuPrincipal(AuthController& authController)
        : auth(authController) {}

    void mostrar() const {
        UI::limpiarPantalla();

        //  Colores ANSI
        const std::string verde  = "\033[1;32m";
        const std::string gris   = "\033[1;90m";
        const std::string blanco = "\033[0;37m";
        const std::string reset  = "\033[0m";

        // Animación de inicio
        const std::string texto = "CARGANDO SISTEMA";
        for (char c : texto) {
            std::cout << verde << c << reset << std::flush;
            Sleep(70);
        }

        std::cout << "\n" << gris << "Inicializando módulos..." << reset;
        for (int i = 0; i < 5; ++i) {
            std::cout << verde << " #" << std::flush;
            Sleep(200);
        }
        Sleep(400);
        UI::limpiarPantalla();

//  Banner ASCII "WELCOME"
std::cout << verde;
std::cout << "##############################################################\n";
std::cout << "##  _    _      _                                         ##\n";
std::cout << "## | |  | |    | |                                        ##\n";
std::cout << "## | |  | | ___| | ___ ___  _ __ ___   ___                ##\n";
std::cout << "## | |/\\| |/ _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\               ##\n";
std::cout << "## \\  /\\  /  __/ | (_| (_) | | | | | |  __/               ##\n";
std::cout << "##  \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|               ##\n";
std::cout << "##############################################################\n";
std::cout << reset;

        // Efecto de parpadeo tipo 
        for (int i = 0; i < 3; ++i) {
            std::cout << "\033[1;92m";
            std::cout << "\n\n######################  BIENVENIDO  #########################" << reset;
            Sleep(250);
            std::cout << "\r                                                            \r" << std::flush;
            Sleep(200);
        }

        // Subtítulo
        std::cout << gris << "\n\n░▒▓  SISTEMA DE ENVIOS GARANTIZADOS S.A ▓▒░\n" << reset;

        // Menú principal
        std::cout << blanco << "\n[1] Iniciar sesión\n";
        std::cout << "[2] Registrarse\n";
        std::cout << "[0] Salir\n\n";
        std::cout << verde << "Seleccione una opción ➤ " << reset;
    }

    // ===== Login =====
    void mostrarLogin() const {
        UI::limpiarPantalla();
        const std::string azul = "\033[1;36m";
        const std::string reset = "\033[0m";

        UI::mostrarTitulo("INICIAR SESIÓN");
        std::cout << azul << "\nEmail: " << reset;
    }

    bool procesarLogin(const std::string& email, const std::string& password) {
        return auth.iniciarSesion(email, password);
    }

    // ===== Registro =====
    void mostrarRegistro() const {
        UI::limpiarPantalla();
        const std::string verde = "\033[1;32m";
        const std::string reset = "\033[0m";

        UI::mostrarTitulo("REGISTRO DE NUEVO USUARIO");
        std::cout << verde << "(El rol por defecto es Cliente)\n\n" << reset;
    }

    bool procesarRegistroInteractivo() {
        const std::string rojo  = "\033[1;31m";
        const std::string verde = "\033[1;32m";
        const std::string reset = "\033[0m";

        std::string nombre, email, password;

        std::cout << "Nombre completo: ";
        std::getline(std::cin, nombre);

        std::cout << "Email: ";
        std::getline(std::cin, email);

        std::cout << "Contraseña: ";
        std::getline(std::cin, password);

        bool ok = auth.registrarUsuario(nombre, email, password, Rol::CLIENTE);
        if (!ok) {
            std::cout << rojo << "\n✗ No se pudo registrar. Revise email/contraseña o si el email ya existe.\n" << reset;
            UI::pausa();
        } else {
            std::cout << verde << "\n✓ Usuario registrado exitosamente. Ya puede iniciar sesión.\n" << reset;
            UI::pausa();
        }
        return ok;
    }

    bool procesarRegistro(const std::string& nombre,
                          const std::string& email,
                          const std::string& password,
                          Rol rol = Rol::CLIENTE)
    {
        return auth.registrarUsuario(nombre, email, password, rol);
    }
};

#endif // MENU_PRINCIPAL_H
