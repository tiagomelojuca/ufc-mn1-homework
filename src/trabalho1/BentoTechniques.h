#ifndef BENTOTECHNIQUES_H_
#define BENTOTECHNIQUES_H_

#include <string>
#include <vector>

#include "TFuncao.h"
#include "FuncoesMatematicas.h"

// ------------------------------------------------------------------------------------------------

namespace BentoTechniques
{
    double EstimaX0(const std::string& expr, double centro, double delta, size_t saltos)
    {
        double x0 = FuncoesMatematicas::Sentinela();

        delta = fabs(delta);

        enum ESinal { NUL, POS, NEG };
        auto Sinal = [](double val) { return val == 0.0 ? NUL : (val < 0.0 ? NEG : POS); };

        std::vector<double> abscissas;

        const size_t nx = 2u * saltos + 1;
        const double xmin = centro - saltos * delta;

        for (size_t i = 0; i < nx; i++)
        {
            abscissas.push_back(xmin + i * delta);
        }

        if (!abscissas.empty()) {
            TFuncao f(expr);

            for (size_t i = 0; i < abscissas.size() - 1; i++)
            {
                if (Sinal(f(abscissas[i])) != Sinal(f(abscissas[i + 1])))
                {
                    x0 = 0.5 * (abscissas[i] + abscissas[i + 1]);
                }
            }
        }

        return x0;
    }
}

// ------------------------------------------------------------------------------------------------

#endif // BENTOTECHNIQUES_H_
