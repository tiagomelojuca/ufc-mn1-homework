#ifndef TCALCULORAIZESNEWTONMODIFICADO_H_
#define TCALCULORAIZESNEWTONMODIFICADO_H_

#include "TCalculoRaizesNewtonRaphson.h"

#include <cfloat>
#include <string>

#include "FabricacaoArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

class TCalculoRaizesNewtonModificado : public TCalculoRaizesNewtonRaphson
{
public:
    TCalculoRaizesNewtonModificado(
        const std::string& f,
        const std::string& f_,
        double x0,
        double err
    ) :
        TCalculoRaizesNewtonRaphson(f, f_, x0, err)
    {
        f_x0 = TCalculoRaizesNewtonRaphson::f_(x0);
    }
    
    TCalculoRaizesNewtonModificado(const TCalculoRaizesNewtonModificado&) = delete;
    TCalculoRaizesNewtonModificado(TCalculoRaizesNewtonModificado&&) = delete;
    TCalculoRaizesNewtonModificado& operator=(const TCalculoRaizesNewtonModificado&) = delete;

protected:
    double DenominadorFuncaoIteracao(double x) override
    {
        return f_x0;
    }

private:
    double f_x0 = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TCALCULORAIZESNEWTONMODIFICADO_H_
