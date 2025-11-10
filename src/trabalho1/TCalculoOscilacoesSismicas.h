#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>

#include "FuncoesGerais.h"
#include "FuncoesMatematicas.h"

// ------------------------------------------------------------------------------------------------

class TCalculoOscilacoesSismicas
{
public:
    TCalculoOscilacoesSismicas() = default;

    double Calcula(double a, double d) const
    {
        return FuncoesMatematicas::Calcula(GeraExpressaoDinamicamente(a), d);
    }

private:
    std::string GeraExpressaoDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo("f(d) = $*e^d - 4*d^2", std::to_string(a).c_str());
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
