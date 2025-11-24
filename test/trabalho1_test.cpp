#include <gtest/gtest.h>

#include <sstream>

#include "trabalho1/BentoTechniques.h"
#include "trabalho1/CalculoOscilacoesSismicas.h"
#include "trabalho1/FuncoesGerais.h"
#include "trabalho1/FuncoesMatematicas.h"
#include "trabalho1/TArvoreSintatica.h"
#include "trabalho1/TImpressoraCalculoOscilacoesSismicas.h"
#include "trabalho1/TParser.h"

// ------------------------------------------------------------------------------------------------

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

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_parsear_expressao_valida)
{
    TParser parser;

    // Casos Basicos
    EXPECT_TRUE(parser.Parse("f(x) = 5.0") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 5,0") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = (5.0)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ((5.0))") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ((5.0) * 2.0)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = x") != nullptr);
    EXPECT_TRUE(parser.Parse("f'(x) = x") != nullptr);
    EXPECT_TRUE(parser.Parse("f''(x) = x") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x)' = x") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x)'' = x") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = sin(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = Sin(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = SIN(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = sen(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = Sen(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = SEN(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = cos(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = Cos(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = COS(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = tan(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = Tan(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = TAN(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = tg(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = Tg(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = TG(x)") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = x + 7") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 3x^2 * sen(x) + 7") != nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 3x^2 * (sen(x) + 7)") != nullptr);

    // Deve sinalizar cadeia invalida
    EXPECT_TRUE(parser.Parse("f(x) =") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = f(x) = 5.0 f(x)=") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 5.0()") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ()5.0") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 5.0 =") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )5.0(") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )5.0") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = (5.0") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 5.0)") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = 5.0(") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ()") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )(") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ()()") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )()(") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ()()()") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )()()(") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ((()))") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = )))(((") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = ())))") == nullptr);
    EXPECT_TRUE(parser.Parse("f(x) = (((()") == nullptr);
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_calcular_expressoes)
{
    const double erro = 0.1;

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 5.0", 999), 5.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 5.0^2", 999), 25.0, erro);

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = e", 999), 2.72, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = e^2", 999), 7.39, erro);

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = x", 123), 123.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = x", 999), 999.0, erro);

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = sen(x)", 0.0), 0.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = sen(x)", 0.785), 0.71, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = sen(x)", 1.57), 1.0, erro);

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = x + 7", 0.0), 7.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = x + 7", 2.0), 9.0, erro);

    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 3x^2 * sen(x) + 7", 2.0), 17.91, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 3x^2 * sen(x) + 7", 3.0), 10.81, erro);
    
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 3x^2 * (sen(x) + 7)", 2.0),  94.91, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(x) = 3x^2 * (sen(x) + 7)", 3.0), 192.81, erro);

    // Funcao do trabalho, testando valor solicitado de a = 1
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 0.0), 1.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 0.5), 0.65, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 1.0), -1.28, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 1.5), -4.52, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 2.0), -8.61, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 2.5), -12.82, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 3.0), -15.91, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 3.5), -15.88, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 4.0), -9.4, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 4.5), 9.02, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 1*e^d - 4*d^2", 5.0), 48.41, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 0.0), 1.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 0.5), 0.65, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 1.0), -1.28, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 1.5), -4.52, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 2.0), -8.61, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 2.5), -12.82, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 3.0), -15.91, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 3.5), -15.88, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 4.0), -9.4, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 4.5), 9.02, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = e^d - 4d^2", 5.0), 48.41, erro);

    // Funcao do trabalho, testando valor arbitrario para conferencia de a = 7
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 0.0), 7.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 0.5), 10.54, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 1.0), 15.03, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 1.5), 22.37, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 2.0), 35.72, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 2.5), 60.28, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 3.0), 104.6, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 3.5), 182.81, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 4.0), 318.19, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 4.5), 549.12, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7*e^d - 4*d^2", 5.0), 938.89, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 0.0), 7.0, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 0.5), 10.54, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 1.0), 15.03, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 1.5), 22.37, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 2.0), 35.72, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 2.5), 60.28, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 3.0), 104.6, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 3.5), 182.81, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 4.0), 318.19, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 4.5), 549.12, erro);
    EXPECT_NEAR(FuncoesMatematicas::Calcula("f(d) = 7e^d - 4d^2", 5.0), 938.89, erro);
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_gerar_strings_a_partir_de_modelo)
{
    const std::string str1 = FuncoesGerais::InstanciaModelo("Foo");
    const std::string str2 = FuncoesGerais::InstanciaModelo("Foo$", "Bar");
    const std::string str3 = FuncoesGerais::InstanciaModelo("Foo$$", "Bar", "Baz");
    const std::string str4 = FuncoesGerais::InstanciaModelo("$", "Foo", "Bar", "Baz");
    const std::string str5 = FuncoesGerais::InstanciaModelo("$ $ $", "Foo", "Bar", "Baz");

    EXPECT_STREQ(str1.c_str(), "Foo");
    EXPECT_STREQ(str2.c_str(), "FooBar");
    EXPECT_STREQ(str3.c_str(), "FooBarBaz");
    EXPECT_STREQ(str4.c_str(), "Foo");
    EXPECT_STREQ(str5.c_str(), "Foo Bar Baz");
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_calcular_oscilacoes_de_ondas_sismicas)
{
    using namespace CalculoOscilacoesSismicas;

    const double erro = 0.000001;

    const TEntradaCalculo entradaPadrao { 1.0, 0.5, 0.0001 };
    EXPECT_NEAR(Calcula(EMetodoCalculo::NEWTON_RAPHSON, entradaPadrao), 0.477507, erro);
    EXPECT_NEAR(Calcula(EMetodoCalculo::NEWTON_MODIFICADO, entradaPadrao), 0.477548, erro);
    EXPECT_NEAR(Calcula(EMetodoCalculo::SECANTE, entradaPadrao), 0.477509, erro);
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_imprimir_resultados)
{
    TImpressoraCalculoOscilacoesSismicas impressora;
    impressora.Adiciona(1.0, 0.5, 0.0001);
    impressora.Adiciona(2.0, 1.2, 0.0001);
    impressora.Adiciona(3.0, 0.5, 0.0001);

    std::stringstream ss;
    ss << "|        a |       d0 |       d1 |  err_adm |     d_nr |"
       << "   err_nr |     d_nm |   err_nm |     d_sc |   err_sc |"
       << std::endl
       << "-------------------------------------------------------"
       << "-"
       << "-------------------------------------------------------"
       << std::endl
       << "| 1.000000 | 0.500000 | 0.600000 | 0.000100 | 0.477507 |"
       << " 0.000001 | 0.477548 | 0.000091 | 0.477509 | 0.000005 |"
       << std::endl
       << "| 2.000000 | 1.200000 | 1.300000 | 0.000100 | 1.260146 |"
       << " 0.000000 | 1.260128 | 0.000053 | 1.260144 | 0.000006 |"
       << std::endl
       << "| 3.000000 | 0.500000 | 0.600000 | 0.000100 | -0.52033 |"
       << " 0.000000 |      nan |      nan | -0.52032 | 0.000012 |"
       << std::endl;
    const std::string quadroResumoEsperado = ss.str();

    EXPECT_STREQ(impressora.Sintetiza().c_str(), quadroResumoEsperado.c_str());
}

// ------------------------------------------------------------------------------------------------

TEST(trabalho1_test, deve_ser_capaz_de_estimar_x0)
{
    const double erro = 0.0001;
    EXPECT_NEAR(CalculoOscilacoesSismicas::EstimaX0(1.0),  0.375, erro);
    EXPECT_NEAR(CalculoOscilacoesSismicas::EstimaX0(2.0),  1.375, erro);
    EXPECT_NEAR(CalculoOscilacoesSismicas::EstimaX0(3.0), -0.625, erro);
}

// ------------------------------------------------------------------------------------------------
