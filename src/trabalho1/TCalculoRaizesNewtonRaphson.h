#ifndef TCALCULORAIZESNEWTONRAPHSON_H_
#define TCALCULORAIZESNEWTONRAPHSON_H_

#include "TCalculoRaizes.h"

#include <cfloat>

#include "FabricacaoArvoreSintatica.h"

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
        f_(FabricacaoArvoreSintatica::Fabrica(f_))
    {
    }
    
    TCalculoRaizesNewtonRaphson(const TCalculoRaizesNewtonRaphson&) = delete;
    TCalculoRaizesNewtonRaphson(TCalculoRaizesNewtonRaphson&&) = delete;
    TCalculoRaizesNewtonRaphson& operator=(const TCalculoRaizesNewtonRaphson&) = delete;

    virtual ~TCalculoRaizesNewtonRaphson()
    {
        delete f_;
    }

    double Busca() const override
    {
        double xi = x0;
        double fx = DBL_MAX;
        do {
            fx = f->Resolve(xi);
            xi = xi - fx / DenominadorFuncaoIteracao(xi);
        } while (fabs(fx) > errAdm);

        return xi;
    }

protected:
    virtual double DenominadorFuncaoIteracao(double x) const
    {
        return f_->Resolve(x);
    }

    TArvoreSintatica* f_ = nullptr;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESNEWTONRAPHSON_H_
