#ifndef TORQUESTRADORES_H_
#define TORQUESTRADORES_H_

#include <iostream>

#include "TMatriz.h"
#include "TSistema.h"
#include "Utilidades.h"

// ------------------------------------------------------------------------------------------------

// Gerencia operacoes de entrada/saida com o usuario
class TOrquestradorES
{
public:
    struct TDadosEntrada { TSistema sis; TSistema::EMetodo met; double err; };
    struct TDadosSaida { TMatriz invA; TMatriz solSis; };

    TOrquestradorES() = default;

    static TDadosEntrada SolicitaDadosEntrada()
    {
        TSistema s = SolicitaSistema();
        TSistema::EMetodo m = SolicitaMetodoResolucao();
        const double e = SolicitaErroAdmissivel();
        ImprimeAvisoCarregamento();

        return { s, m, e };
    }

    static void Imprime(const TDadosSaida& dadosSaida)
    {
        ImprimeResultados(dadosSaida.invA, dadosSaida.solSis);
    }

private:
    static TSistema SolicitaSistema()
    {
        const uint8_t n = SolicitaOrdemSistema();

        const TMatriz A = SolicitaMatrizCoeficientes(n);
        const TMatriz b = SolicitaVetorTermosIndependentes(n);

        return TSistema { A, b };
    }

    static TSistema::EMetodo SolicitaMetodoResolucao()
    {
        std::cout << "Digite 0 para encontrar a inversa por Gauss/Jacobi ou 1 para Gauss/Seidel: ";
        int m;
        std::cin >> m;

        return m == 0 ? TSistema::EMetodo::GAUSS_JACOBI : TSistema::EMetodo::GAUSS_SEIDEL;
    }

    static double SolicitaErroAdmissivel()
    {
        std::cout << "Digite a precisao desejada: ";
        double err;
        std::cin >> err;

        return err;
    }

    static uint8_t SolicitaOrdemSistema()
    {
        std::cout << "Digite a ordem do sistema: ";
        int _n;
        std::cin >> _n;
        const auto n = static_cast<uint8_t>(_n);

        return n;
    }

    static TMatriz SolicitaMatrizCoeficientes(uint8_t n)
    {
        TMatriz A { n, n };
        std::cout << "Digite os termos a_ij da matriz de coeficientes: " << '\n';
        for (int lin = 1; lin <= n; lin++)
        {
            for (int col = 1; col <= n; col++)
            {
                std::cout << "    A[" << lin << "][" << col << "]: ";

                double a_lc;
                std::cin >> a_lc;
                A.Valor(lin, col, a_lc);
            }
        }

        return A;
    }

    static TMatriz SolicitaVetorTermosIndependentes(uint8_t n)
    {
        TMatriz b { n, 1 };
        std::cout << "Digite os termos b_i do vetor independente: " << '\n';
        for (int lin = 1; lin <= n; lin++)
        {
            std::cout << "    b[" << lin << "]: ";

            double bi;
            std::cin >> bi;
            b.Valor(lin, 1, bi);
        }

        return b;
    }

    static void ImprimeAvisoCarregamento()
    {
        std::cout << "------------------------------------------------------------\n";
        std::cout << "[AVISO] Calculando resultados, isso pode demorar um pouco...\n";
        std::cout << "------------------------------------------------------------\n";
    }

    static void ImprimeResultados(const TMatriz& invA, const TMatriz& x)
    {
        std::cout << "[A^(-1)]\n";
        std::cout << Utilidades::ToString(invA) << '\n';

        std::cout << "[x]\n";
        std::cout << Utilidades::ToString(x) << '\n';
    }
};

// ------------------------------------------------------------------------------------------------

#endif // TORQUESTRADORES_H_
