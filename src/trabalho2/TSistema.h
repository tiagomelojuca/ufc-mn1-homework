#ifndef TSISTEMA_H_
#define TSISTEMA_H_

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

    TMatriz Resolve(EMetodo m) const
    {
        return m == EMetodo::GAUSS_JACOBI
            ? ResolvePorGaussJacobi()
            : ResolvePorGaussSeidel();
    }

private:
    TMatriz ResolvePorGaussJacobi() const
    {
        // ToDo
        return TMatriz(0u, 0u);
    }

    TMatriz ResolvePorGaussSeidel() const
    {
        // ToDo
        return TMatriz(0u, 0u);
    }

    TMatriz _A;
    TMatriz _b;
};

// ------------------------------------------------------------------------------------------------

#endif // TSISTEMA_H_
