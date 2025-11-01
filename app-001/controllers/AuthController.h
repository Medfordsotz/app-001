#ifndef AUTH_CONTROLLER_H
#define AUTH_CONTROLLER_H

#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "../models/Usuario.h"
#include "../models/Enums.h"     
#include "../utils/Utilidades.h"  

class AuthController {
private:
    std::vector<Usuario*>& usuarios;
    Usuario* usuarioActual;
    int& siguienteId;

    // Helpers internos
    static std::string trim(const std::string& s) {
        size_t b = s.find_first_not_of(" \t\n\r");
        size_t e = s.find_last_not_of(" \t\n\r");
        if (b == std::string::npos) return "";
        return s.substr(b, e - b + 1);
    }
    static std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return s;
    }
    static std::string normalizarEmail(const std::string& email) {
        return toLower(trim(email));
    }

public:
    
    AuthController(std::vector<Usuario*>& users, int& nextId)
        : usuarios(users), usuarioActual(nullptr), siguienteId(nextId) {}

    // Verificar disponibilidad de email (público por si lo usan las vistas)
    bool esEmailDisponible(const std::string& email) const {
        std::string e = normalizarEmail(email);
        for (auto* u : usuarios) {
            if (u && toLower(u->getEmail()) == e) return false;
        }
        return true;
    }

    // login Iniciar sesión
    bool iniciarSesion(const std::string& email, const std::string& password) {
        std::string e = normalizarEmail(email);
        for (auto* usuario : usuarios) {
            if (!usuario) continue;
            if (toLower(usuario->getEmail()) == e &&
                usuario->verificarPassword(password) &&
                usuario->estaActivo()) {
                usuarioActual = usuario;
                return true;
            }
        }
        return false;
    }

    // Cerrar sesión
    void cerrarSesion() {
        usuarioActual = nullptr;
    }

    // Datos para Registrar nuevo usuario
    bool registrarUsuario(const std::string& nombre,
                          const std::string& email,
                          const std::string& password,
                          Rol rol = Rol::CLIENTE)
    {
        const std::string e = normalizarEmail(email);

        // Validaciones
        if (!Validaciones::validarEmail(e) ||
            nombre.empty() ||
            password.length() < 4) {
            return false;
        }
        if (!esEmailDisponible(e)) {
            return false;
        }

        // Crearcion de  usuario
        Usuario* nuevoUsuario = new Usuario(siguienteId++, nombre, e, password, rol);
        usuarios.push_back(nuevoUsuario);
        return true;
    }

    // Cambio contraseña
    bool cambiarPassword(const std::string& passwordAntiguo,
                         const std::string& passwordNuevo)
    {
        if (!usuarioActual) return false;
        if (passwordNuevo.length() < 4) return false;

        if (usuarioActual->verificarPassword(passwordAntiguo)) {
            usuarioActual->setPassword(passwordNuevo);
            return true;
        }
        return false;
    }

   
    Usuario* getUsuarioActual() const { return usuarioActual; }
    bool hayUsuarioAutenticado() const { return usuarioActual != nullptr; }


    Rol getRolUsuarioActual() const {
        return usuarioActual ? usuarioActual->getRol() : Rol::CLIENTE;
    }
};

#endif
