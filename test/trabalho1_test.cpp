#include <gtest/gtest.h>

#include "trabalho1/TArvoreSintatica.h"
#include "trabalho1/FuncoesMatematicas.h"

TEST(trabalho1_test, deve_ser_capaz_calcular_arvore)
{
    const double erro = 0.1;
    {
        // f(x) nao existe
        TArvoreSintatica arvore { nullptr };
        EXPECT_NEAR(arvore.Resolve(999), 0.0, erro);
    }
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
        // f(x) = sen(x)
        TArvoreSintatica arvore {
            new TNohOperacaoUnaria { TNohOperacaoUnaria::EOperacao::SENO, new TNohLiteral }
        };
        EXPECT_NEAR(arvore.Resolve(0.0), 0.0, erro);
        EXPECT_NEAR(arvore.Resolve(0.785), 0.71, erro);
        EXPECT_NEAR(arvore.Resolve(1.57), 1.0, erro);
    }
    {
        // f(x) = x + 7
        TArvoreSintatica arvore {
            new TNohOperacaoBinaria {
                TNohOperacaoBinaria::EOperacao::SOMA,
                new TNohLiteral,
                new TNohConstante { 7.0 }
            }
        };
        EXPECT_NEAR(arvore.Resolve(0.0), 7.0, erro);
        EXPECT_NEAR(arvore.Resolve(2.0), 9.0, erro);
    }
    {
        // f(x) = 3x^2 * sen(x) + 7
        auto raiz = new TNohOperacaoBinaria {
            TNohOperacaoBinaria::EOperacao::SOMA,
            new TNohOperacaoBinaria {
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
                new TNohOperacaoUnaria {
                    TNohOperacaoUnaria::EOperacao::SENO,
                    new TNohLiteral
                }
            },
            new TNohConstante { 7.0 }
        };

        TArvoreSintatica arvore { raiz };
        EXPECT_NEAR(arvore.Resolve(2.0), 17.91, erro);
        EXPECT_NEAR(arvore.Resolve(3.0), 10.81, erro);
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

TEST(trabalho1_test, deve_ser_capaz_de_construir_arvore_sintatica)
{
    const double erro = 0.1;
    TParser parser;
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = 5.0");
        EXPECT_NEAR(arvore->Resolve(999), 5.0, erro);
        delete arvore;
    }
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = x");
        EXPECT_NEAR(arvore->Resolve(123), 123.0, erro);
        EXPECT_NEAR(arvore->Resolve(999), 999.0, erro);
        delete arvore;
    }
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = sen(x)");
        EXPECT_NEAR(arvore->Resolve(0.0), 0.0, erro);
        EXPECT_NEAR(arvore->Resolve(0.785), 0.71, erro);
        EXPECT_NEAR(arvore->Resolve(1.57), 1.0, erro);
        delete arvore;
    }
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = x + 7");
        EXPECT_NEAR(arvore->Resolve(0.0), 7.0, erro);
        EXPECT_NEAR(arvore->Resolve(2.0), 9.0, erro);
        delete arvore;
    }
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = 3x^2 * sen(x) + 7");
        EXPECT_NEAR(arvore->Resolve(2.0), 17.91, erro);
        EXPECT_NEAR(arvore->Resolve(3.0), 10.81, erro);
        delete arvore;
    }
    {
        TArvoreSintatica* arvore = parser.Parse("f(x) = 3x^2 * (sen(x) + 7)");
        EXPECT_NEAR(arvore->Resolve(2.0),  94.91, erro);
        EXPECT_NEAR(arvore->Resolve(3.0), 192.81, erro);
        delete arvore;
    }
}
