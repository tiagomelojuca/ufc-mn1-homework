#include <gtest/gtest.h>

#include "trabalho2/TMatriz.h"
#include "trabalho2/TSistema.h"

// ------------------------------------------------------------------------------------------------

TEST(trabalho2_test, deve_ser_capaz_de_calcular_inversa)
{
    const float tol = 0.01f;

    TMatriz A(3u, 3u, { { 2,  1, 3 },
                        { 0, -1, 1 },
                        { 1,  0, 3 } });
    TMatriz inversaEsperadaA(3u, 3u, { {  3/2,  3/2, -2 },
                                       { -1/2, -3/2,  1 },
                                       { -1/2, -1/2,  1 } });

    EXPECT_TRUE(A.Inversa().Igual(inversaEsperadaA, tol));
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho2_test, deve_ser_capaz_de_resolver_sistema)
{
    const float tol = 0.01f;
    {
        TMatriz A(3u, 3u, { { 3, 2,  4 },
                            { 1, 1,  2 },
                            { 4, 3, -2 } });
        TMatriz b(3u, 1u, { { 1 },
                            { 2 },
                            { 3 } });
        TMatriz xEsperado(3u, 1u, { { -3 },
                                    {  5 },
                                    {  0 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { {  1, -3,  2 },
                            { -2,  8, -1 },
                            {  4, -6,  5 } });
        TMatriz b(3u, 1u, { {  11 },
                            { -15 },
                            {  29 } });
        TMatriz xEsperado(3u, 1u, { {  2 },
                                    { -1 },
                                    {  3 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { { 3, -4,  1 },
                            { 1,  2,  2 },
                            { 4,  0, -3 } });
        TMatriz b(3u, 1u, { {  9 },
                            {  3 },
                            { -2 } });
        TMatriz xEsperado(3u, 1u, { {  1 },
                                    { -1 },
                                    {  2 } });

        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { { 5, 3, 1 },
                            { 5, 6, 1 },
                            { 1, 6, 7 } });
        TMatriz b(3u, 1u, { { 1 },
                            { 2 },
                            { 3 } });
        TMatriz xEsperado(3u, 1u, { { -1/34 },
                                    {   1/3 },
                                    {  5/34 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
}

// ------------------------------------------------------------------------------------------------
