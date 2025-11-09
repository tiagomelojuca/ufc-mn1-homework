#ifndef FUNCOESMATEMATICAS_H_
#define FUNCOESMATEMATICAS_H_

#include <string>
#include <limits>

#include "FabricacaoArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

namespace FuncoesMatematicas
{

    constexpr float Sentinela()
    {
        return std::numeric_limits<float>::max();
    }

    // --------------------------------------------------------------------------------------------

    float Calcula(const std::string& expr, float x)
    {
        float resultado = Sentinela();

        if (TArvoreSintatica* arv = FabricacaoArvoreSintatica::Fabrica(expr))
        {
            resultado = arv->Resolve(x);
            delete arv;
        }

        return resultado;
    }

}

// ------------------------------------------------------------------------------------------------

#endif
