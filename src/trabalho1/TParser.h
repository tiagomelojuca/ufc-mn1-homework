#ifndef TPARSER_H_
#define TPARSER_H_

#include "IParser.h"
#include "TArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

class TParser : public IParser
{
public:
    TParser() = default;
    virtual ~TParser() = default;

    TNoh* Parse(const std::string& expr) override
    {
        return nullptr;
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TPARSER_H_
