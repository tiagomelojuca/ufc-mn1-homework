#include <iostream>

#include "TImpressoraCalculoOscilacoesSismicas.h"

int main()
{
    TImpressoraCalculoOscilacoesSismicas impressora;
    
    double err;
    std::cout << "Digite a precisao desejada: ";
    std::cin >> err;

    int i = 1;

    double a;
    std::cout << "Digite o i-esimo valor de a, ou 0 para finalizar" << '\n';
    do {
        std::cout << "    a" << i << ": ";
        std::cin >> a;

        if (a != 0.0)
        {
            impressora.Adiciona(a, 0.0, err);
            i++;
        }
    } while (a != 0.0);

    std::cout << impressora.Sintetiza();

    return 0;
}
