#ifndef TCALCULORAIZESNEWTONRAPHSON_H_
#define TCALCULORAIZESNEWTONRAPHSON_H_

#include "TCalculoRaizes.h"

#include <cfloat>

#include "TFuncao.h"

// ------------------------------------------------------------------------------------------------

class TCalculoRaizesNewtonRaphson : public TCalculoRaizes
{
public:
    TCalculoRaizesNewtonRaphson(
        const std::string& f,
        const std::string& f_,
        double x0,
        double err
    ) :
        TCalculoRaizes(f, x0, err),
        f_(f_)
    {
    }
    
    TCalculoRaizesNewtonRaphson(const TCalculoRaizesNewtonRaphson&) = delete;
    TCalculoRaizesNewtonRaphson(TCalculoRaizesNewtonRaphson&&) = delete;
    TCalculoRaizesNewtonRaphson& operator=(const TCalculoRaizesNewtonRaphson&) = delete;

    double Busca() override
    {
        double xi = x0;
        double fx = f(xi);
        do {
            xi = xi - fx / DenominadorFuncaoIteracao(xi);
            fx = f(xi);
        } while (++nIteracoes < maxIteracoes && fabs(fx) > errAdm);

        return fabs(fx) <= errAdm ? xi : Sentinela();
    }

protected:
    virtual double DenominadorFuncaoIteracao(double x)
    {
        return f_(x);
    }

    TFuncao f_;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESNEWTONRAPHSON_H_
