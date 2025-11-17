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
        double fx = DBL_MAX;
        do {
            xi = (xa * f->Resolve(xi) - xi * f->Resolve(xa)) / (f->Resolve(xi) - f->Resolve(xa));
        } while (fabs(f->Resolve(xi)) > errAdm);

        return xi;
    }

protected:
    double x1 = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESSECANTE_H_
