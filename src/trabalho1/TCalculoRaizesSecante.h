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

    double Busca() const override
    {
        double xa = x0; // x anterior
        double xi = x1;
        double fxi = f->Resolve(xi);
        double fxa = f->Resolve(xa);
        do {
            const double xn = (xa * fxi - xi * fxa) / (fxi - fxa);
            xa = xi;
            fxa = fxi;
            xi = xn;
            fxi = f->Resolve(xi);
        } while (fabs(fxi) > errAdm);

        return xi;
    }

protected:
    double x1 = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESSECANTE_H_
