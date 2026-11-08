#include "ControlDeLinea.h"
#include "Fresadora.h"
#include "Torno.h"
#include "Pulidora.h"
#include "InspeccionVisual.h"
#include "BandaTransportadora.h"

#include <iostream>
#include <exception>

int main() {
    try {
        Torno torno("Torno T-01");
        Fresadora fresadora("Fresadora F-01");
        Pulidora pulidora("Pulidora P-01");
        InspeccionVisual inspeccion("Inspeccion I-01");
        BandaTransportadora banda("Banda B-01");

        ControlDeLinea linea;

        linea.agregar(&torno);
        linea.agregar(&fresadora);
        linea.agregar(&pulidora);
        linea.agregar(&inspeccion);
        linea.agregar(&banda);
        

        std::cout << "--- Turno de produccion ---\n";
        linea.ejecutarTurno(2);

        std::cout << "--- Reporte de la linea ---\n";
        linea.imprimirReporte();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}