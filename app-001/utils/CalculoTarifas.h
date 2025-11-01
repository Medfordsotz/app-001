#ifndef CALCULO_TARIFAS_H
#define CALCULO_TARIFAS_H

#include "Utilidades.h"
#include "../models/Enums.h"   // TARIFA_BASE, MULTIPLICADOR_NOCTURNO

// ============================================================
// CalculadoraTarifas
// ------------------------------------------------------------
//  - Calcula la tarifa base según la hora y el día de la semana.
//  - Domingo: servicio no disponible (devuelve -1).
//  - Nocturno: antes de 8:00 o después de 18:00 → tarifa * multiplicador.
// ============================================================

class CalculadoraTarifas {
private:
    inline static float tarifaBase = TARIFA_BASE;
    inline static float multiplicadorNocturno = MULTIPLICADOR_NOCTURNO;

public:
    // --------------------------------------------
    // Calcular tarifa según fecha/hora
    // --------------------------------------------
    static float calcularTarifa(const Fecha& fecha) {
        if (fecha.esDomingo()) {
            // Servicio no disponible los domingos
            return -1.0f;
        }

        float tarifa = tarifaBase;

        // Tarifa nocturna (antes de 8 a.m. o después de 6 p.m.)
        if (fecha.esHorarioNocturno()) {
            tarifa *= multiplicadorNocturno;
        }

        return tarifa;
    }

    // --------------------------------------------
    // Getters / Setters de configuración
    // --------------------------------------------
    static void setTarifaBase(float nuevaTarifa) {
        if (nuevaTarifa > 0.0f)
            tarifaBase = nuevaTarifa;
    }

    static void setMultiplicadorNocturno(float nuevoMultiplicador) {
        if (nuevoMultiplicador > 0.0f)
            multiplicadorNocturno = nuevoMultiplicador;
    }

    static float getTarifaBase() { return tarifaBase; }
    static float getMultiplicadorNocturno() { return multiplicadorNocturno; }
};

#endif // CALCULO_TARIFAS_H
