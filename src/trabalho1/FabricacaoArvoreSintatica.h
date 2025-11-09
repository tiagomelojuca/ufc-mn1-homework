#ifndef FABRICACAOARVORESINTATICA_H_
#define FABRICACAOARVORESINTATICA_H_

#include "TParser.h"

// ------------------------------------------------------------------------------------------------

namespace FabricacaoArvoreSintatica
{

    TArvoreSintatica* Fabrica(const std::string& expr)
    {
        TParser parser;
        TNoh* raiz = parser.Parse(expr);

        return raiz != nullptr ? new TArvoreSintatica(raiz) : nullptr;
    }

}

// ------------------------------------------------------------------------------------------------

#endif // FABRICACAOARVORESINTATICA_H_
