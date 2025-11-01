#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include "../models/Enums.h"

// ===============================================
//              ESTRUCTURA FECHA
// ===============================================
struct Fecha {
    int dia{1}, mes{1}, anio{1970}, hora{0}, minuto{0};

    // Construye con la fecha/hora actual del sistema (local)
    Fecha() {
        std::time_t ahora = std::time(nullptr);
        std::tm* tl = std::localtime(&ahora);
        if (tl) {
            dia    = tl->tm_mday;
            mes    = tl->tm_mon + 1;
            anio   = tl->tm_year + 1900;
            hora   = tl->tm_hour;
            minuto = tl->tm_min;
        }
    }

    // Construye una fecha específica
    Fecha(int d, int m, int a, int h = 0, int min = 0)
        : dia(d), mes(m), anio(a), hora(h), minuto(min) {}

    // Convierte la fecha almacenada a time_t (localtime)
    std::time_t to_time_t() const {
        std::tm t{};
        t.tm_mday = dia;
        t.tm_mon  = mes - 1;       // 0..11
        t.tm_year = anio - 1900;   // desde 1900
        t.tm_hour = hora;
        t.tm_min  = minuto;
        t.tm_sec  = 0;
        t.tm_isdst = -1;           
        return std::mktime(&t);  
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << dia << "/"
            << std::setw(2) << mes << "/"
            << anio << " "
            << std::setw(2) << hora << ":"
            << std::setw(2) << minuto;
        return oss.str();
    }

    // Horario nocturno según constantes globales
    bool esHorarioNocturno() const {
        return hora < HORA_INICIO_DIURNO || hora >= HORA_FIN_DIURNO;
    }

    // Domingo basado en la fecha almacenada (¡arregla el bug!)
    bool esDomingo() const {
        std::time_t tt = to_time_t();
        std::tm* tl = std::localtime(&tt);
        return tl ? (tl->tm_wday == 0) : false; // 0 = domingo
    }
};

// ===============================================
//                NAMESPACE UTILIDADES
// ===============================================
namespace Utilidades {
    inline Fecha fechaActual() { return Fecha(); }

    // (Opcional) sumar días
    inline Fecha sumarDias(const Fecha& f, int dias) {
        std::time_t tt = f.to_time_t();
        tt += static_cast<long long>(dias) * 24LL * 60LL * 60LL;
        std::tm* tl = std::localtime(&tt);
        if (!tl) return f;
        return Fecha(tl->tm_mday, tl->tm_mon + 1, tl->tm_year + 1900, tl->tm_hour, tl->tm_min);
    }
}

// ===============================================
//               FUNCIONES DE VALIDACIÓN
// ===============================================
namespace Validaciones {

    inline bool validarEmail(const std::string& email) {
        // Validación simple: contiene '@' y '.'
        auto at = email.find('@');
        auto dot = email.find('.', at == std::string::npos ? 0 : at);
        return at != std::string::npos && dot != std::string::npos && at < dot;
    }

    inline bool validarPeso(float peso) {
        return peso > 0.0f && peso <= PESO_MAXIMO;
    }

    inline bool validarTelefono(const std::string& telefono) {
        // Regla simple: al menos 8 caracteres
        return telefono.length() >= 8;
    }

    inline void limpiarBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// ===============================================
//               FUNCIONES DE INTERFAZ
// ===============================================
namespace UI {
    inline void limpiarPantalla() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
    }

    inline void pausa() {
        std::cout << "\nPresione ENTER para continuar...";
        // si venimos de un >> num, hay que limpiar el salto anterior
        if (std::cin.peek() == '\n') std::cin.get();
        std::cin.get();
    }

    inline void mostrarLinea() {
        std::cout << "========================================\n";
    }

    inline void mostrarTitulo(const std::string& titulo) {
        mostrarLinea();
        std::cout << "  " << titulo << "\n";
        mostrarLinea();
    }
}

#endif // UTILIDADES_H
