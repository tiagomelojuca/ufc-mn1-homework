#ifndef UTILIDADES_H_
#define UTILIDADES_H_

#include <iostream>
#include <string>

#include "TMatriz.h"

// ------------------------------------------------------------------------------------------------

namespace Utilidades
{
    void Imprime(const TMatriz& m)
    {
        std::string _m;

        for (uint8_t l = 1; l <= m.Linhas(); l++)
        {
            for (uint8_t c = 1; c <= m.Colunas(); c++)
            {
                _m += std::to_string(m.Valor(l, c));
                _m += " ";
            }
            _m += "\n";
        }

        std::cout << _m;
    }
}

// ------------------------------------------------------------------------------------------------

#endif // UTILIDADES_H_
