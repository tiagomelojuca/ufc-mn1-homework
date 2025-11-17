#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iomanip>

#include "FuncoesGerais.h"
#include "TCalculoRaizesNewtonRaphson.h"
#include "TCalculoRaizesNewtonModificado.h"
#include "TCalculoRaizesSecante.h"

// ------------------------------------------------------------------------------------------------

class TCalculoOscilacoesSismicas
{
public:
    enum class EMetodoCalculo { NEWTON_RAPHSON, NEWTON_MODIFICADO, SECANTE };

    TCalculoOscilacoesSismicas() = default;

    std::string Sintetiza(double a) const
    {
        std::stringstream ss;

        const double dNR = Calcula(EMetodoCalculo::NEWTON_RAPHSON, a);
        const double dNM = Calcula(EMetodoCalculo::NEWTON_MODIFICADO, a);
        const double dSc = Calcula(EMetodoCalculo::SECANTE, a);

        ss << "> " << GeraExpressaoDinamicamente(a) << std::endl
           << "    NEWTON-RAPHSON    : " << dNR << std::endl
           << "    NEWTON-MODIFICADO : " << dNM << std::endl
           << "    SECANTE           : " << dSc << std::endl;

        return ss.str();
    }

    std::string Sintetiza() const
    {
        std::vector<double> entradas;
        entradas.push_back(1.0);
        entradas.push_back(2.0);
        entradas.push_back(3.0);

        std::stringstream ss;

            ss << "| " << std::right
               << std::setw(10) << std::fixed << std::setprecision(10) << "a" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "d0" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "d1" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "err" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "d_nr" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "d_nm" << " | "
               << std::setw(10) << std::fixed << std::setprecision(10) << "d_sc" << " |"
               << std::endl;

        for (double a : entradas)
        {
            const double dNR = Calcula(EMetodoCalculo::NEWTON_RAPHSON, a);
            const double dNM = Calcula(EMetodoCalculo::NEWTON_MODIFICADO, a);
            const double dSc = Calcula(EMetodoCalculo::SECANTE, a);

            ss << "| " << std::right
               << std::setw(8) << std::fixed << std::setprecision(8) << a << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << 0.5 << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << 0.6 << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << 0.0001 << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dNR << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dNM << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dSc << " |"
               << std::endl;
        }

        return ss.str();
    }

    double Calcula(EMetodoCalculo metodo, double a) const
    {
        const double x0 = 0.5;
        const double x1 = 0.6;
        const double errAdm = 0.0001;

        const std::string f = GeraExpressaoDinamicamente(a);
        const std::string f_ = GeraDerivadaDinamicamente(a);

        return FabricaCalculo(metodo, f, f_, x0, x1, errAdm)->Busca();
    }

private:
    std::unique_ptr<TCalculoRaizes> FabricaCalculo(
        EMetodoCalculo metodo,
        const std::string& f,
        const std::string& f_,
        double x0,
        double x1,
        double errAdm
    ) const
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

    std::string GeraExpressaoDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }

    std::string GeraDerivadaDinamicamente(double a) const
    {
        return FuncoesGerais::InstanciaModelo(modeloDerivadaFuncaoAplicacaoImagem, std::to_string(a).c_str());
    }

    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
