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
        double err
    ) :
        TCalculoRaizes(f, x0, err)
    {
    }

    virtual double Busca() const = 0;

protected:
    double x1 = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESSECANTE_H_
