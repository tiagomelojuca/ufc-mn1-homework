#ifndef TCALCULORAIZES_H_
#define TCALCULORAIZES_H_

#include <string>

#include "FuncoesMatematicas.h"
#include "TFuncao.h"

// ------------------------------------------------------------------------------------------------

class TCalculoRaizes
{
public:
    TCalculoRaizes(
        const std::string& f,
        double x0,
        double err
    ) :
        x0(x0),
        errAdm(err),
        f(f)
    {
    }

    TCalculoRaizes(const TCalculoRaizes&) = delete;
    TCalculoRaizes(TCalculoRaizes&&) = delete;
    TCalculoRaizes& operator=(const TCalculoRaizes&) = delete;

    virtual double Busca() = 0;

    size_t NumeroIteracoesRealizadas() const
    {
        return nIteracoes;
    }

    void NumeroMaximoIteracoes(size_t n)
    {
        maxIteracoes = n;
    }

    double Sentinela() const
    {
        return FuncoesMatematicas::Sentinela();
    }

protected:
    double x0 = 0.0;
    double errAdm = 0.0;

    size_t nIteracoes = 0u;
    size_t maxIteracoes = 10u;

    TFuncao f;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZES_H_
