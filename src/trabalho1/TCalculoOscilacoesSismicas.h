#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>

#include "FuncoesGerais.h"
#include "FuncoesMatematicas.h"

// ------------------------------------------------------------------------------------------------

class TCalculoOscilacoesSismicas
{
public:
    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";

    TCalculoOscilacoesSismicas() = default;

    double Calcula(double a, double d) const
    {
        return FuncoesMatematicas::Calcula(GeraExpressaoDinamicamente(a), d);
    }

private:
    std::string GeraExpressaoDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }

    std::string GeraDerivadaDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloDerivadaFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
