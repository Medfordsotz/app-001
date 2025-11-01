#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include <iostream>
#include "../utils/Utilidades.h"  

class Mensaje {
private:
    int id;
    int idRemitente;
    int idDestinatario;
    int idSolicitud;
    std::string contenido;
    Fecha fecha;     // Se asume que Fecha tiene constructor por defecto (fecha actual)
    bool leido;

public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    Mensaje(int id,
             int remitente,
             int destinatario,
             int solicitud,
             const std::string& msg)
        : id(id),
          idRemitente(remitente),
          idDestinatario(destinatario),
          idSolicitud(solicitud),
          contenido(msg),
          fecha(Utilidades::fechaActual()),  //funcion para la fecha actual 
          leido(false) {}

    // ------------------------------------------------------------
    // Getters
    // ------------------------------------------------------------
    int getId() const { return id; }
    int getIdRemitente() const { return idRemitente; }
    int getIdDestinatario() const { return idDestinatario; }
    int getIdSolicitud() const { return idSolicitud; }
    const std::string& getContenido() const { return contenido; }
    const Fecha& getFecha() const { return fecha; }
    bool estaLeido() const { return leido; }

    
    void marcarLeido()    { leido = true; }
    void marcarNoLeido()  { leido = false; }

    // ------------------------------------------------------------
    // Mostrar mensaje completo
    // ------------------------------------------------------------
    void mostrar() const {
        std::cout << "┌────────────────────────────────────────┐\n";
        std::cout << "│ " << (leido ? "[LEÍDO]" : "[NUEVO MENSAJE]") << "\n";
        std::cout << "│ Fecha: " << fecha.toString() << "\n";
        std::cout << "│ Solicitud #" << idSolicitud << "\n";
        std::cout << "├────────────────────────────────────────┤\n";
        std::cout << "│ " << contenido << "\n";
        std::cout << "└────────────────────────────────────────┘\n";
    }

    // ------------------------------------------------------------
    // Mostrar resumen corto (lista de mensajes)
    // ------------------------------------------------------------
 void mostrarResumen() const {
    std::cout << "[" << (leido ? (const char*)" " : (const char*)u8"●") << "] "
              << fecha.toString()
              << " - Solicitud #" << idSolicitud << "\n";
}
};

#endif
 // MENSAJE_H
