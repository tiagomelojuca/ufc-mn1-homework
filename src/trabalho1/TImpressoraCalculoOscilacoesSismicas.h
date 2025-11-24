#ifndef TIMPRESSORACALCULOOSCILACOESSISMICAS_H_
#define TIMPRESSORACALCULOOSCILACOESSISMICAS_H_

#include <vector>

#include "CalculoOscilacoesSismicas.h"
#include "TFuncao.h"
#include "TTabela.h"

// ------------------------------------------------------------------------------------------------

class TImpressoraCalculoOscilacoesSismicas {
public:
    TImpressoraCalculoOscilacoesSismicas() = default;

    void Adiciona(double a, double x0, double err)
    {
        entradas.push_back({ a, x0, err });
    }

    std::string Sintetiza() const
    {
        return Sintetiza(entradas);
    }

private:
    using TEntradaCalculo = CalculoOscilacoesSismicas::TEntradaCalculo;
    using EMetodoCalculo = CalculoOscilacoesSismicas::EMetodoCalculo;

    std::string Sintetiza(const std::vector<TEntradaCalculo>& entradas) const
    {
        TTabela t { entradas.size() + 1, 10u, 8u };
        PreencheCabecalho(t);

        for (size_t i = 2; i <= entradas.size() + 1; i++)
        {
            PreencheLinha(t, i, entradas[i - 2]);
        }

        return t.Gera();
    }

    void PreencheCabecalho(TTabela& t) const
    {
        t.DefineCabecalho(1);
        t.Define(1, 1, "a");
        t.Define(1, 2, "d0");
        t.Define(1, 3, "d1");
        t.Define(1, 4, "err_adm");
        t.Define(1, 5, "d_nr");
        t.Define(1, 6, "err_nr");
        t.Define(1, 7, "d_nm");
        t.Define(1, 8, "err_nm");
        t.Define(1, 9, "d_sc");
        t.Define(1, 10, "err_sc");
    }

    void PreencheLinha(TTabela& t, size_t l, const TEntradaCalculo& e) const
    {
        using namespace CalculoOscilacoesSismicas;

        auto f = TFuncao(GeraExpressaoDinamicamente(e.a));
        const double dNR = Calcula(EMetodoCalculo::NEWTON_RAPHSON, e);
        const double dNM = Calcula(EMetodoCalculo::NEWTON_MODIFICADO, e);
        const double dSC = Calcula(EMetodoCalculo::SECANTE, e);
        
        t.Define(l, 1, e.a);
        t.Define(l, 2, e.x0);
        t.Define(l, 3, EstimaX1(e.x0));
        t.Define(l, 4, e.err);
        t.Define(l, 5, dNR);
        t.Define(l, 6, fabs(f(dNR)));
        t.Define(l, 7, dNM);
        t.Define(l, 8, fabs(f(dNM)));
        t.Define(l, 9, dSC);
        t.Define(l, 10, fabs(f(dSC)));
    }

    std::vector<TEntradaCalculo> entradas;
};

// ------------------------------------------------------------------------------------------------

#endif // TIMPRESSORACALCULOOSCILACOESSISMICAS_H_
