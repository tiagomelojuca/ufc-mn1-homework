#include <gtest/gtest.h>

#include "trabalho2/TMatriz.h"
#include "trabalho2/TSistema.h"
#include "trabalho2/Utilidades.h"

// ------------------------------------------------------------------------------------------------

TEST(trabalho2_test, deve_ser_capaz_de_calcular_inversa)
{
    const float tol = 0.01f;

    TMatriz A(3u, 3u, { { 2.0,  1.0, 3.0 },
                        { 0.0, -1.0, 1.0 },
                        { 1.0,  0.0, 3.0 } });
    TMatriz inversaEsperadaA(3u, 3u, { {  3.0/2.0,  3.0/2.0, -2.0 },
                                       { -1.0/2.0, -3.0/2.0,  1.0 },
                                       { -1.0/2.0, -1.0/2.0,  1.0 } });

    EXPECT_TRUE(A.Inversa().Igual(inversaEsperadaA, tol));
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho2_test, deve_ser_capaz_de_resolver_sistema)
{
    const float tol = 0.01f;
    {
        TMatriz A(3u, 3u, { { 3.0, 2.0,  4.0 },
                            { 1.0, 1.0,  2.0 },
                            { 4.0, 3.0, -2.0 } });
        TMatriz b(3u, 1u, { { 1.0 },
                            { 2.0 },
                            { 3.0 } });
        TMatriz xEsperado(3u, 1u, { { -3.0 },
                                    {  5.0 },
                                    {  0.0 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { {  1.0, -3.0,  2.0 },
                            { -2.0,  8.0, -1.0 },
                            {  4.0, -6.0,  5.0 } });
        TMatriz b(3u, 1u, { {  11.0 },
                            { -15.0 },
                            {  29.0 } });
        TMatriz xEsperado(3u, 1u, { {  2.0 },
                                    { -1.0 },
                                    {  3.0 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { { 3.0, -4.0,  1.0 },
                            { 1.0,  2.0,  2.0 },
                            { 4.0,  0.0, -3.0 } });
        TMatriz b(3u, 1u, { {  9.0 },
                            {  3.0 },
                            { -2.0 } });
        TMatriz xEsperado(3u, 1u, { {  1.0 },
                                    { -1.0 },
                                    {  2.0 } });

        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
    {
        TMatriz A(3u, 3u, { { 5.0, 3.0, 1.0 },
                            { 5.0, 6.0, 1.0 },
                            { 1.0, 6.0, 7.0 } });
        TMatriz b(3u, 1u, { { 1.0 },
                            { 2.0 },
                            { 3.0 } });
        TMatriz xEsperado(3u, 1u, { { -1.0/34.0 },
                                    {   1.0/3.0 },
                                    {  5.0/34.0 } });
        
        TSistema s(A, b);
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_JACOBI).Igual(xEsperado, tol));
        EXPECT_TRUE(s.Resolve(TSistema::EMetodo::GAUSS_SEIDEL).Igual(xEsperado, tol));
    }
}

// ------------------------------------------------------------------------------------------------
