#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <iostream>
#include "Enums.h"
#include "../utils/Utilidades.h"

class Usuario {
protected:
    int id;
    std::string nombre;
    std::string direccion;
    std::string telefono;
    std::string email;
    std::string password;
    Rol rol;
    bool activo;
    Fecha fechaRegistro;

public:
    // Constructor
    Usuario(int id,
            const std::string& nombre,
            const std::string& email,
            const std::string& password,
            Rol rol)
        : id(id),
          nombre(nombre),
          direccion(),
          telefono(),
          email(email),
          password(password),
          rol(rol),
          activo(true),
          fechaRegistro(Utilidades::fechaActual()) {}

    // Getters 
    int getId() const { return id; }
    const std::string& getNombre() const { return nombre; }
    const std::string& getDireccion() const { return direccion; }
    const std::string& getTelefono() const { return telefono; }
    const std::string& getEmail() const { return email; }
    Rol getRol() const { return rol; }
    bool estaActivo() const { return activo; }
    const Fecha& getFechaRegistro() const { return fechaRegistro; }

    // Setters
    void setNombre(const std::string& nom) { nombre = nom; }
    void setDireccion(const std::string& dir) { direccion = dir; }
    void setTelefono(const std::string& tel) { telefono = tel; }
    void setEmail(const std::string& em) { email = em; }
    void setPassword(const std::string& pass) { password = pass; }
    void setRol(Rol r) { rol = r; }                 // <- necesario para AdminController

    // Métodos
    bool verificarPassword(const std::string& pass) const {
        return password == pass; // TODO: en futuro, usar hash/sal
    }

    void activar() { activo = true; }
    void desactivar() { activo = false; }

    // Vistas
    void mostrarInfo() const {
        std::cout << "┌────────────────────────────────────┐\n";
        std::cout << "│ ID: " << id << "\n";
        std::cout << "│ Nombre: " << nombre << "\n";
        std::cout << "│ Email: " << email << "\n";
        std::cout << "│ Rol: " << rolToString(rol) << "\n";
        std::cout << "│ Estado: " << (activo ? "✓ Activo" : "✗ Inactivo") << "\n";
        if (!direccion.empty()) std::cout << "│ Dirección: " << direccion << "\n";
        if (!telefono.empty())  std::cout << "│ Teléfono: " << telefono << "\n";
        std::cout << "│ Registro: " << fechaRegistro.toString() << "\n";
        std::cout << "└────────────────────────────────────┘\n";
    }

    void mostrarResumen() const {
        std::cout << "[" << id << "] " << nombre << " - "
                  << email << " (" << rolToString(rol) << ") "
                  << (activo ? "[Activo]" : "[Inactivo]") << "\n";
    }
};

#endif 
// USUARIO_H
