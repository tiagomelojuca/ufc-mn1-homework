#ifndef TARVORESINTATICA_H_
#define TARVORESINTATICA_H_

#include "TNoh.h"

// ------------------------------------------------------------------------------------------------

class TArvoreSintatica
{
public:
    TArvoreSintatica(TNoh* raiz) : _raiz(raiz) {}

    ~TArvoreSintatica()
    {
        delete _raiz;
    }

    float Resolve(float x)
    {
        auto AplicaValorX = [x](TNoh& noh)
        {
            if (auto nohVariavel = dynamic_cast<TNohLiteral*>(&noh))
            {
                nohVariavel->Valor(x);
            }
        };
        VisitaEmOrdem(_raiz, AplicaValorX);

        return _raiz != nullptr ? _raiz->Valor() : 0.0;
    }

private:
    void VisitaEmOrdem(TNoh* x, std::function<void(TNoh&)> Visita) const
    {
        if (x != nullptr)
        {
            VisitaEmOrdem(x->Esq(), Visita);
            Visita(*x);
            VisitaEmOrdem(x->Dir(), Visita);
        }
    }

    TNoh* _raiz = nullptr;
};

// ------------------------------------------------------------------------------------------------

#endif // TARVORESINTATICA_H_
