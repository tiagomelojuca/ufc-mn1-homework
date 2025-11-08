#ifndef IPARSER_H_
#define IPARSER_H_

#include <string>

// ------------------------------------------------------------------------------------------------
class TNoh;
// ------------------------------------------------------------------------------------------------

class IParser
{
public:
    IParser() = default;
    virtual ~IParser() = default;

    virtual TNoh* Parse(const std::string& expr) = 0;
};

// ------------------------------------------------------------------------------------------------

#endif // IPARSER_H_
