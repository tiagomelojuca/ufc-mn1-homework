#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>

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
        std::string expr;

        expr += "f(d) = ";
        expr += std::to_string(a);
        expr += "*e^d - 4*d^2";

        return expr;
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
