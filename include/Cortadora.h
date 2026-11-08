#ifndef CORTADORA_H
#define CORTADORA_H

#include "Maquina.h"

namespace planta {

class Cortadora : public Maquina {
public:
    explicit Cortadora(const std::string& nombre);

    int tiempoCicloSeg() const override;

private:
    void ejecutarCiclo() override;
};

} 

#endif