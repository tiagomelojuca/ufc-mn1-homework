#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>

#include "FuncoesGerais.h"
#include "FuncoesMatematicas.h"

// ------------------------------------------------------------------------------------------------

class TCalculoOscilacoesSismicas
{
public:
    static constexpr const char* modeloFuncao = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncao = "f'(d) = $*e^d - 8*d";

    TCalculoOscilacoesSismicas() = default;

    double Calcula(double a, double d) const
    {
        return FuncoesMatematicas::Calcula(GeraExpressaoDinamicamente(a), d);
    }

private:
    std::string GeraExpressaoDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloFuncao, std::to_string(a).c_str());
    }

    std::string GeraDerivadaDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloDerivadaFuncao, std::to_string(a).c_str());
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
