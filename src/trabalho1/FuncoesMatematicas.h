#ifndef FUNCOESMATEMATICAS_H_
#define FUNCOESMATEMATICAS_H_

#include <string>
#include <limits>

#include "TParser.h"

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

        TParser parser;
        TNoh* raiz = parser.Parse(expr);
        if (raiz != nullptr)
        {
            TArvoreSintatica arvore(raiz);
            resultado = arvore.Resolve(x);
        }

        return resultado;
    }

}

// ------------------------------------------------------------------------------------------------

#endif
