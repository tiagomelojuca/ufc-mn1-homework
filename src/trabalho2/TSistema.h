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

    static TMatriz Inversa(const TMatriz& A, EMetodo m, double errAdm)
    {
        const uint8_t n = A.Linhas();
        if (A.Colunas() != n)
        {
            return TMatriz(0, 0);
        }

        // Conforme diz o enunciado, se A eh uma matriz nxn que possui como inversa
        // uma matriz A^(-1) entao A*A^(-1) = I, onde I eh a matriz identidade, e uma
        // maneira de se achar A^(-1) eh achar-se as colunas de A^(-1) uma por vez:
        // A*A^(-1)_1 = { 1 0 ... 0 }t
        // A*A^(-1)_2 = { 0 1 ... 0 }t
        // A*A^(-1)_n = { 0 0 ... 1 }t
        // Onde A^(-1)_i eh a i-esima coluna de A
        // Como A e I sao conhecidos, isso consiste basicamente em determinar A^(-1)_i,
        // ou seja, resolver um sistema no qual x = A^(-1)_i e b = I
        const TMatriz I = TMatriz::Identidade(n);

        TMatriz invA { n, n };

        for (uint8_t col = 1; col <= n; col++)
        {
            TMatriz b { n, 1 }; // coluna correspondente de I
            for (uint8_t lin = 1; lin <= n; lin++)
            {
                b.Valor(lin, 1, I.Valor(lin, col));
            }

            TSistema sis { A, b };
            const TMatriz x = sis.Resolve(m, errAdm); // A^(-1)_col

            for (uint8_t lin = 1; lin <= n; lin++)
            {
                invA.Valor(lin, col, x.Valor(lin, 1));
            }
        }

        return invA;
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
        const uint8_t n = _A.Linhas();
        TMatriz D { n, n }; // elementos da diagonal da matriz dos coeficientes
        TMatriz E { n, n }; // particao triangular inferior matriz dos coeficientes
        TMatriz F { n, n }; // particao triangular superior matriz dos coeficientes
        for (int l = 1; l <= n; l++)
        {
            for (int c = 1; c <= n; c++)
            {
                const double a_lc = _A.Valor(l, c);
                if (l == c)
                {
                    D.Valor(l, c, a_lc);
                }
                else if (l > c)
                {
                    E.Valor(l, c, a_lc);
                }
                else
                {
                    F.Valor(l, c, a_lc);
                }
            }
        }

        // A inversa de uma matriz diagonal eh trivial de calcular
        TMatriz Dinv { n, n };
        for (uint8_t i = 1; i <= n; i++)
        {
            Dinv.Valor(i, i, 1.0 / D.Valor(i, i));
        }

        TMatriz C = Dinv.Produto(E).Produto(-1.0);
        TMatriz Cant = Dinv.Produto(F).Produto(-1.0);
        TMatriz g = Dinv.Produto(_b);

        TMatriz x0 = _b;
        for (int l = 1; l <= g.Linhas(); l++)
        {
            double x0_l1 = x0.Valor(l, 1) / _A.Valor(l, l);
            x0.Valor(l, 1, x0_l1);
        }

        uint8_t iter = 0u;
        double err = std::numeric_limits<double>::max();
        TMatriz xAnt { g.Linhas(), g.Colunas() };
        TMatriz x = x0;
        while (err > errAdm && iter < _maxIter)
        {
            xAnt = x;

            // x = C.Produto(x) + Cant(xAnt) + g,
            // mas nao tem como fazer essa operacao atomicamente
            for (uint8_t l = 1; l <= n; l++)
            {
                double parcelaC = 0.0;
                for (int c = 1; c < l; c++)
                {
                    parcelaC += C.Valor(l, c) * x.Valor(c, 1);
                }

                double parcelaCant = 0.0;
                for (int c = l + 1; c <= n; c++)
                {
                    parcelaCant += Cant.Valor(l, c) * xAnt.Valor(c, 1);
                }

                const double parcelaG = g.Valor(l, 1);

                const double x_l1 = parcelaC + parcelaCant + parcelaG;

                x.Valor(l, 1, x_l1);
            }

            err = Erro(x, xAnt);
            iter++;
        }

        return x;
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
