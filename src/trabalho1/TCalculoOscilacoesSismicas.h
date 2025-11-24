#ifndef TCALCULOOSCILACOESSISMICAS_H_
#define TCALCULOOSCILACOESSISMICAS_H_

#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "FuncoesGerais.h"
#include "TCalculoRaizesNewtonRaphson.h"
#include "TCalculoRaizesNewtonModificado.h"
#include "TCalculoRaizesSecante.h"
#include "TTabela.h"

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

    std::string Sintetiza(TEntradaCalculo entrada) const
    {
        std::stringstream ss;

        const double dNR = Calcula(EMetodoCalculo::NEWTON_RAPHSON, entrada);
        const double dNM = Calcula(EMetodoCalculo::NEWTON_MODIFICADO, entrada);
        const double dSc = Calcula(EMetodoCalculo::SECANTE, entrada);

        ss << "> " << GeraExpressaoDinamicamente(entrada.a) << std::endl
           << "    NEWTON-RAPHSON    : " << dNR << std::endl
           << "    NEWTON-MODIFICADO : " << dNM << std::endl
           << "    SECANTE           : " << dSc << std::endl;

        return ss.str();
    }

    std::string Sintetiza(const std::vector<TEntradaCalculo>& entradas) const
    {
        TTabela t { entradas.size() + 1, 7u, 8u };
        PreencheCabecalho(t);

        for (size_t i = 2; i <= entradas.size() + 1; i++)
        {
            PreencheLinha(t, i, entradas[i - 2]);
        }

        return t.Gera();
    }

    std::string Sintetiza() const
    {
        std::vector<TEntradaCalculo> entradas;
        entradas.push_back({ 1.0, 0.5, 0.0001 });
        entradas.push_back({ 2.0, 1.2, 0.0001 });
        entradas.push_back({ 3.0, 0.5, 0.0001 });

        return Sintetiza(entradas);
    }

private:
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

    void PreencheCabecalho(TTabela& t) const
    {
        t.DefineCabecalho(1);
        t.Define(1, 1, "a");
        t.Define(1, 2, "d0");
        t.Define(1, 3, "d1");
        t.Define(1, 4, "err");
        t.Define(1, 5, "d_nr");
        t.Define(1, 6, "d_nm");
        t.Define(1, 7, "d_sc");
    }

    void PreencheLinha(TTabela& t, size_t l, const TEntradaCalculo& e) const
    {
        t.Define(l, 1, e.a);
        t.Define(l, 2, e.x0);
        t.Define(l, 3, EstimaX1(e.x0));
        t.Define(l, 4, e.err);
        t.Define(l, 5, Calcula(EMetodoCalculo::NEWTON_RAPHSON, e));
        t.Define(l, 6, Calcula(EMetodoCalculo::NEWTON_MODIFICADO, e));
        t.Define(l, 7, Calcula(EMetodoCalculo::SECANTE, e));
    }

    static constexpr const char* modeloFuncaoOriginal = "f(d) = $*e^d - 4*d^2";
    static constexpr const char* modeloDerivadaFuncaoOriginal = "f'(d) = $*e^d - 8*d";

    static constexpr const char* modeloFuncaoAplicacaoImagem = "f(d) = $*e^d - 4*d^2 - 0.7";
    static constexpr const char* modeloDerivadaFuncaoAplicacaoImagem = "f'(d) = $*e^d - 8*d";
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULOOSCILACOESSISMICAS_H_
