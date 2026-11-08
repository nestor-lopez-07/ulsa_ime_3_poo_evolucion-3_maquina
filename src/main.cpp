#include "ControlDeLinea.h"
#include "Fresadora.h"
#include "Torno.h"
#include "Pulidora.h"

#include <iostream>

int main() {
    Torno torno("Torno T-01");
    Fresadora fresadora("Fresadora F-01");
    Pulidora pulidora("Pulidora P-01");
 

    ControlDeLinea linea;

    linea.agregar(&torno);
    linea.agregar(&fresadora);
    linea.agregar(&pulidora);
   

    std::cout << "--- Turno de produccion ---\n";
    linea.ejecutarTurno(2);

    std::cout << "--- Reporte de la linea ---\n";
    linea.imprimirReporte();

    return 0;
}