#ifndef TTABELA_H_
#define TTABELA_H_

#include <string>
#include <sstream>

#include "FuncoesGerais.h"
#include "TMatriz.h"

// ------------------------------------------------------------------------------------------------

class TTabela : public TMatriz<std::string>
{
public:
    TTabela(size_t linhas, size_t colunas) : TMatriz(linhas, colunas)
    {
        Inicializa(CelulaVazia());
    }

    std::string Gera() const
    {
        std::stringstream ss;

        for (size_t linha = 0; linha < _linhas; linha++)
        {
            ss << "|";

            for (size_t coluna = 0; coluna < _colunas; coluna++)
            {
                ss << _matriz[linha][coluna] << "|";
            }

            ss << std::endl;
        }

        return ss.str();
    }

private:
    std::string CelulaVazia() const
    {
        const size_t tamanhoPaddings = 2u;
        const size_t tamanho = tamanhoCelulas + tamanhoPaddings;

        std::string celulaVazia;
        for (size_t i = 0; i < tamanho; i++) celulaVazia += " ";

        return celulaVazia;
    }

    size_t tamanhoCelulas = 7u;
};

// ------------------------------------------------------------------------------------------------

#endif // TTABELA_H_
