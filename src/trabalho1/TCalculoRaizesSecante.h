#ifndef TCALCULORAIZESSECANTE_H_
#define TCALCULORAIZESSECANTE_H_

#include "TCalculoRaizes.h"

// ------------------------------------------------------------------------------------------------

class TCalculoRaizesSecante : public TCalculoRaizes
{
public:
    TCalculoRaizesSecante(
        const std::string& f,
        double x0,
        double x1,
        double err
    ) :
        TCalculoRaizes(f, x0, err),
        x1(x1)
    {
    }

    double Busca() override
    {
        double xa = x0; // x anterior
        double xi = x1;
        double fxi = f(xi);
        double fxa = f(xa);
        do {
            const double xn = (xa * fxi - xi * fxa) / (fxi - fxa);
            xa = xi;
            fxa = fxi;
            xi = xn;
            fxi = f(xi);
        } while (++nIteracoes < maxIteracoes && fabs(fxi) > errAdm);

        return fabs(fxi) <= errAdm ? xi : Sentinela();
    }

protected:
    double x1 = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESSECANTE_H_
