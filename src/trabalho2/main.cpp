#include "TOrquestradorES.h"

// ------------------------------------------------------------------------------------------------
#define SEM_ERRO 0
// ------------------------------------------------------------------------------------------------

TOrquestradorES::TDadosSaida Calcula(const TOrquestradorES::TDadosEntrada& dadosEntrada)
{
    const TMatriz& A = dadosEntrada.sis.MatrizCoeficientes();
    const TSistema::EMetodo metodo = dadosEntrada.met;
    const double errAdm = dadosEntrada.err;

    const TMatriz invA = TSistema::Inversa(A, metodo, errAdm);
    const TMatriz& b = dadosEntrada.sis.VetorTermosIndependentes();
    TMatriz x = invA.Produto(b);

    return { invA, x };
}

// ------------------------------------------------------------------------------------------------

int main()
{
    const auto dadosEntrada = TOrquestradorES::SolicitaDadosEntrada();
    const auto dadosSaida = Calcula(dadosEntrada);
    TOrquestradorES::Imprime(dadosSaida);

    return SEM_ERRO;
}

// ------------------------------------------------------------------------------------------------
