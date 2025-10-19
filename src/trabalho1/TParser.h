#ifndef TPARSER_H_
#define TPARSER_H_

#include <string>

#include "TArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

class TParser
{
public:
    TParser() = default;

    TArvoreSintatica* Parse(const std::string& expr)
    {
        return new TArvoreSintatica { nullptr };
    }
};

// ------------------------------------------------------------------------------------------------

#endif
