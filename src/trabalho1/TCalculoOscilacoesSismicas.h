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

    struct TEntradaCalculo
    {
        double a;
        double x0;
        double err;
    };

    TCalculoOscilacoesSismicas() = default;

    std::string Sintetiza(TEntradaCalculo e) const
    {
        std::stringstream ss;

        const double dNR = Calcula(EMetodoCalculo::NEWTON_RAPHSON, e);
        const double dNM = Calcula(EMetodoCalculo::NEWTON_MODIFICADO, e);
        const double dSc = Calcula(EMetodoCalculo::SECANTE, e);

        ss << "> " << GeraExpressaoDinamicamente(e.a) << std::endl
           << "    NEWTON-RAPHSON    : " << dNR << std::endl
           << "    NEWTON-MODIFICADO : " << dNM << std::endl
           << "    SECANTE           : " << dSc << std::endl;

        return ss.str();
    }

    std::string Sintetiza() const
    {
        std::vector<TEntradaCalculo> entradas;
        entradas.push_back({ 1.0, 0.5, 0.0001 });
        entradas.push_back({ 2.0, 1.2, 0.0001 });
        entradas.push_back({ 3.0, 0.5, 0.0001 });

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

        for (TEntradaCalculo e : entradas)
        {
            const std::string dNR = StrCalc(EMetodoCalculo::NEWTON_RAPHSON, e);
            const std::string dNM = StrCalc(EMetodoCalculo::NEWTON_MODIFICADO, e);
            const std::string dSc = StrCalc(EMetodoCalculo::SECANTE, e);

            ss << "| " << std::right
               << std::setw(8) << std::fixed << std::setprecision(8) << e.a << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << e.x0 << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << EstimaX1(e.x0) << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << e.err << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dNR << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dNM << " | "
               << std::setw(8) << std::fixed << std::setprecision(8) << dSc << " |"
               << std::endl;
        }

        return ss.str();
    }

private:
    std::string StrCalc(EMetodoCalculo m, TEntradaCalculo e) const
    {
        const double d = Calcula(m, e);
        if (d == FuncoesMatematicas::Sentinela())
        {
            return "NaN";
        }

        return std::to_string(d);
    }

    double Calcula(EMetodoCalculo metodo, TEntradaCalculo entrada) const
    {
        const double x1 = EstimaX1(entrada.x0);

        return Calcula(metodo, entrada.a, entrada.x0, x1, entrada.err);
    }

    double Calcula(
        EMetodoCalculo metodo,
        double a,
        double x0,
        double x1,
        double errAdm
    ) const
    {
        const std::string f = GeraExpressaoDinamicamente(a);
        const std::string f_ = GeraDerivadaDinamicamente(a);

        return FabricaCalculo(metodo, f, f_, x0, x1, errAdm)->Busca();
    }

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

    double EstimaX1(double x0, double delta = 0.1) const
    {
        return x0 + delta;
    }

    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
