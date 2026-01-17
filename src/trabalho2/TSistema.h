#ifndef TSISTEMA_H_
#define TSISTEMA_H_

#include <cmath>

#include "TMatriz.h"

// ------------------------------------------------------------------------------------------------

class TSistema
{
public:
    enum class EMetodo { GAUSS_JACOBI, GAUSS_SEIDEL };

    TSistema() = delete;
    TSistema(const TSistema&) = delete;
    TSistema(TSistema&&) = delete;

    TSistema(const TMatriz& A, const TMatriz& b) : _A(A), _b(b) {}

    TMatriz Resolve(EMetodo m, double errAdm) const
    {
        return m == EMetodo::GAUSS_JACOBI
            ? ResolvePorGaussJacobi(errAdm)
            : ResolvePorGaussSeidel(errAdm);
    }

private:
    TMatriz ResolvePorGaussJacobi(double errAdm) const
    {
        TMatriz C = _A;
        for (int l = 1; l <= C.Linhas(); l++)
        {
            for (int c = 1; c <= C.Colunas(); c++)
            {
                double c_lc = c == l ? 0.0 : C.Valor(l, c) / -_A.Valor(l, l);
                C.Valor(l, c, c_lc);
            }
        }

        TMatriz g = _b;
        for (int l = 1; l <= g.Linhas(); l++)
        {
            double g_l1 = g.Valor(l, 1) / _A.Valor(l, l);
            g.Valor(l, 1, g_l1);
        }

        uint8_t iter = 0u;
        double err = std::numeric_limits<double>::max();
        TMatriz xAnt { g.Linhas(), g.Colunas() };
        TMatriz x = g; // costuma-se usar x0 como o proprio g, economiza 1 iteracao
        while (err > errAdm && iter < _maxIter)
        {
            xAnt = x;
            x = C.Produto(x).Soma(g);
            err = Erro(x, xAnt);
            iter++;
        }

        return x;
    }

    TMatriz ResolvePorGaussSeidel(double errAdm) const
    {
        // ToDo
        return TMatriz(0u, 0u);
    }

    double Erro(const TMatriz& x, const TMatriz& xAnt) const
    {
        const TMatriz diff = x.Soma(xAnt.Produto(-1.0));
        const double d = ArgMax(diff); // distancia, norma infinita

        return d / ArgMax(x);
    }

    double ArgMax(const TMatriz& M) const
    {
        double max = -1.0;

        for (int l = 1; l <= M.Linhas(); l++)
        {
            const double m = fabs(M.Valor(l, 1));
            if (m > max)
            {
                max = m;
            }
        }

        return max;
    }

    TMatriz _A;
    TMatriz _b;

    uint8_t _maxIter = 255u;
};

// ------------------------------------------------------------------------------------------------

#endif // TSISTEMA_H_
