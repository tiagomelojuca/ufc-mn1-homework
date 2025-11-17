#ifndef TFUNCAO_H_
#define TFUNCAO_H_

#include <string>

#include "TArvoreSintatica.h"
#include "FabricacaoArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

class TFuncao
{
public:
    TFuncao() = delete;
    TFuncao(const std::string& f) : _expr(f), _arvore(FabricacaoArvoreSintatica::Fabrica(f)) {}
    TFuncao(const TFuncao& outra) = delete;
    TFuncao(TFuncao&& outra) = delete;
    TFuncao& operator=(const TFuncao&) = delete;

    ~TFuncao()
    {
        delete _arvore;
    }

    double operator()(double x)
    {
        return _arvore->Resolve(x);
    }

private:
    std::string _expr;
    TArvoreSintatica* _arvore = nullptr;
};

// ------------------------------------------------------------------------------------------------

#endif // TFUNCAO_H_
