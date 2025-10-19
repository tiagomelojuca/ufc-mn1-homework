#include <gtest/gtest.h>

#include "trabalho1/TArvoreSintatica.h"

TEST(trabalho1_test, deve_ser_capaz_calcular_arvore)
{
    const double erro = 0.1;
    {
        // f(x) = 5.0
        TArvoreSintatica arvore { new TNohConstante { 5.0 } };
        EXPECT_NEAR(arvore.Resolve(999), 5.0, erro);
    }
    {
        // f(x) = x
        TArvoreSintatica arvore { new TNohLiteral };
        EXPECT_NEAR(arvore.Resolve(123), 123.0, erro);
        EXPECT_NEAR(arvore.Resolve(999), 999.0, erro);
    }
    {
        // f(x) = 3x^2 * (sen(x) + 7)
        auto raiz = new TNohOperacaoBinaria {
            TNohOperacaoBinaria::EOperacao::MULTIPLICACAO,
            new TNohOperacaoBinaria {
                TNohOperacaoBinaria::EOperacao::MULTIPLICACAO,
                new TNohConstante { 3.0 },
                new TNohOperacaoBinaria {
                    TNohOperacaoBinaria::EOperacao::EXPONENCIAL,
                    new TNohLiteral,
                    new TNohConstante { 2.0 }
                }
            },
            new TNohOperacaoBinaria {
                TNohOperacaoBinaria::EOperacao::SOMA,
                new TNohConstante { 7.0 },
                new TNohOperacaoUnaria {
                    TNohOperacaoUnaria::EOperacao::SENO,
                    new TNohLiteral
                }
            }
        };

        TArvoreSintatica arvore { raiz };
        EXPECT_NEAR(arvore.Resolve(2.0),  94.91, erro);
        EXPECT_NEAR(arvore.Resolve(3.0), 192.81, erro);
    }
}
