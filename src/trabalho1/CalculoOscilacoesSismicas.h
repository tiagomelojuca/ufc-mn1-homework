#ifndef CALCULOOSCILACOESSISMICAS_H_
#define CALCULOOSCILACOESSISMICAS_H_

#include <string>
#include <sstream>
#include <memory>

#include "BentoTechniques.h"
#include "FuncoesGerais.h"
#include "TCalculoRaizesNewtonRaphson.h"
#include "TCalculoRaizesNewtonModificado.h"
#include "TCalculoRaizesSecante.h"

// ------------------------------------------------------------------------------------------------

namespace CalculoOscilacoesSismicas
{
    enum class EMetodoCalculo { NEWTON_RAPHSON, NEWTON_MODIFICADO, SECANTE };
    struct TEntradaCalculo { double a; double x0; double err; };

    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";

    std::string GeraExpressaoDinamicamente(double a)
    {
        return FuncoesGerais::InstanciaModelo(modeloFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }

    std::string GeraDerivadaDinamicamente(double a)
    {
        return FuncoesGerais::InstanciaModelo(modeloDerivadaFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }

    std::unique_ptr<TCalculoRaizes> FabricaCalculo(
        EMetodoCalculo metodo,
        const std::string& f,
        const std::string& f_,
        double x0,
        double x1,
        double errAdm
    )
    {
        TCalculoRaizes* calculo = nullptr;

        if (metodo == EMetodoCalculo::NEWTON_RAPHSON)
        {
            calculo = new TCalculoRaizesNewtonRaphson(f, f_, x0, errAdm);
        }
        else if (metodo == EMetodoCalculo::NEWTON_MODIFICADO)
        {
            calculo = new TCalculoRaizesNewtonModificado(f, f_, x0, errAdm);
        }
        else
        {
            calculo = new TCalculoRaizesSecante(f, x0, x1, errAdm);
        }

        return std::move(std::unique_ptr<TCalculoRaizes>(calculo));
    }

    double Calcula(
        EMetodoCalculo metodo,
        double a,
        double x0,
        double x1,
        double errAdm
    )
    {
        const std::string f = GeraExpressaoDinamicamente(a);
        const std::string f_ = GeraDerivadaDinamicamente(a);

        return FabricaCalculo(metodo, f, f_, x0, x1, errAdm)->Busca();
    }

    double EstimaX0(double a)
    {
        return BentoTechniques::EstimaX0(GeraExpressaoDinamicamente(a), 0.0, 0.25, 6u);
    }

    double EstimaX1(double x0, double delta = 0.1)
    {
        return x0 + delta;
    }

    double Calcula(EMetodoCalculo metodo, TEntradaCalculo entrada)
    {
        const double x1 = EstimaX1(entrada.x0);

        return Calcula(metodo, entrada.a, entrada.x0, x1, entrada.err);
    }
};

// ------------------------------------------------------------------------------------------------

#endif // CALCULOOSCILACOESSISMICAS_H_
