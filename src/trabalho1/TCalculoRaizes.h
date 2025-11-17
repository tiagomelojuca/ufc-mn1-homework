#ifndef TCALCULORAIZES_H_
#define TCALCULORAIZES_H_

#include <string>

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

protected:
    double x0 = 0.0;
    double errAdm = 0.0;

    TFuncao f;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZES_H_
