#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>

#include "FuncoesGerais.h"
#include "TCalculoRaizesNewtonRaphson.h"
#include "TCalculoRaizesNewtonModificado.h"

// ------------------------------------------------------------------------------------------------

class TCalculoOscilacoesSismicas
{
public:
    enum class EMetodoCalculo { NEWTON_RAPHSON, NEWTON_MODIFICADO, SECANTE };

    TCalculoOscilacoesSismicas() = default;

    double Calcula(double a, EMetodoCalculo metodo) const
    {
        const std::string f = GeraExpressaoDinamicamente(a);
        const std::string f_ = GeraDerivadaDinamicamente(a);

        if (metodo == EMetodoCalculo::NEWTON_RAPHSON)
        {
            TCalculoRaizesNewtonRaphson calculo(f, f_, 0.5, 0.0001);
            return calculo.Busca();
        }

        if (metodo == EMetodoCalculo::NEWTON_MODIFICADO)
        {
            TCalculoRaizesNewtonModificado calculo(f, f_, 0.5, 0.0001);
            return calculo.Busca();
        }

        return 0.0;
    }

private:
    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";

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
