#ifndef ADMIN_CONTROLLER_H
#define ADMIN_CONTROLLER_H

#include <vector>
#include <iostream>
#include <algorithm>    
#include "../models/Usuario.h"
#include "../models/Solicitud.h"
#include "../models/Enums.h"       
#include "../utils/CalculoTarifas.h"

class AdminController {
private:
    std::vector<Usuario*>& usuarios;
    std::vector<Solicitud*>& solicitudes;

public:
    
    AdminController(std::vector<Usuario*>& users,
                    std::vector<Solicitud*>& sols)
        : usuarios(users), solicitudes(sols) {}

  
    void listarUsuarios() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║     USUARIOS REGISTRADOS               ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n\n";

        if (usuarios.empty()) {
            std::cout << "No hay usuarios registrados.\n";
            return;
        }
        for (const auto& usuario : usuarios) {
        
            usuario->mostrarResumen();
        }
    }

    Usuario* buscarUsuarioPorId(int id) const {
        for (auto* usuario : usuarios) {
            if (usuario && usuario->getId() == id) {
                return usuario;
            }
        }
        return nullptr;
    }

    Usuario* buscarUsuarioPorEmail(const std::string& email) const {
        for (auto* usuario : usuarios) {
            if (usuario && usuario->getEmail() == email) {
                return usuario;
            }
        }
        return nullptr;
    }

    // Alta de usuario: el ID  debe de relacionar
    // verificacion de correo.
    Usuario* crearUsuario(int id,
                          const std::string& nombre,
                          const std::string& email,
                          const std::string& password,
                          Rol rol)
    {
        if (buscarUsuarioPorEmail(email) != nullptr) {
            std::cout << "No se pudo crear. Ya existe un usuario con ese email.\n";
            return nullptr;
        }
        auto* nuevo = new Usuario(id, nombre, email, password, rol);
        usuarios.push_back(nuevo);
        std::cout << "Usuario creado con ID " << id << ".\n";
        return nuevo;
    }

    // Baja de usuario por ID. Protegemos al admin base
    // admin (id=1) 
    bool eliminarUsuarioPorId(int id) {
    
        if (id == 1) {
            std::cout << "No se puede eliminar el administrador por defecto (ID 1).\n";
            return false;
        }

        bool eliminado = false;
        auto it = std::remove_if(usuarios.begin(), usuarios.end(),
            [&](Usuario* u) {
                if (u && u->getId() == id) {
                    delete u;
                    eliminado = true;
                    return true;
                }
                return false;
            });
        usuarios.erase(it, usuarios.end());

        if (eliminado) {
            std::cout << "Usuario con ID " << id << " eliminado.\n";
        } else {
            std::cout << "No se encontró usuario con ID " << id << ".\n";
        }
        return eliminado;
    }

    // Cambiar rol de un usuario existente
    bool cambiarRolUsuario(int id, Rol nuevoRol) {
        Usuario* u = buscarUsuarioPorId(id);
        if (!u) {
            std::cout << "No se encontró el usuario con ID " << id << ".\n";
            return false;
        }
        u->setRol(nuevoRol);
        std::cout << "Rol actualizado para el usuario " << id << ".\n";
        return true;
    }

    //  Actualizaciones  de roles de Usuario
    bool actualizarNombreUsuario(int id, const std::string& nuevoNombre) {
        Usuario* u = buscarUsuarioPorId(id);
        if (!u) return false;
        u->setNombre(nuevoNombre);
        return true;
    }

    bool actualizarEmailUsuario(int id, const std::string& nuevoEmail) {
        if (buscarUsuarioPorEmail(nuevoEmail) != nullptr) {
            std::cout << "Ya existe un usuario con ese email.\n";
            return false;
        }
        Usuario* u = buscarUsuarioPorId(id);
        if (!u) return false;
        u->setEmail(nuevoEmail);
        return true;
    }


    void listarSolicitudes() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║          SOLICITUDES GLOBALES          ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n\n";

        if (solicitudes.empty()) {
            std::cout << "No hay solicitudes registradas.\n";
            return;
        }

        for (const auto& s : solicitudes) {
            s->mostrarResumen();
        }
    }

    // Búsqueda de solicitud por ID
    Solicitud* buscarSolicitudPorId(int id) const {
        for (auto* s : solicitudes) {
            if (s && s->getId() == id) return s;
        }
        return nullptr;
    }

   
    int contarUsuariosPorRol(Rol rol) const {
        int total = 0;
        for (auto* u : usuarios) {
            if (u && u->getRol() == rol) ++total;
        }
        return total;
    }

    void reporteResumen() const {
        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout <<   "║            REPORTE RESUMEN             ║\n";
        std::cout <<   "╚════════════════════════════════════════╝\n";

        std::cout << "Usuarios totales: " << usuarios.size() << "\n";
        std::cout << "  - Clientes:     " << contarUsuariosPorRol(Rol::CLIENTE) << "\n";
        std::cout << "  - Mensajeros:   " << contarUsuariosPorRol(Rol::MENSAJERO) << "\n";
        std::cout << "  - Controladores:" << contarUsuariosPorRol(Rol::CONTROLADOR) << "\n";
        std::cout << "  - Administrador:" << contarUsuariosPorRol(Rol::ADMINISTRADOR) << "\n";
        std::cout << "Solicitudes totales: " << solicitudes.size() << "\n\n";
    }
};

#endif

// ADMIN_CONTROLLER_H
