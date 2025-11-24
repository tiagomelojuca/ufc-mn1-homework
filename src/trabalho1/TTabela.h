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
    TTabela(
        size_t linhas,
        size_t colunas,
        size_t tamanhoCelulas
    ) :
        TMatriz(linhas, colunas),
        tamanhoCelulas(tamanhoCelulas),
        linhaCabecalho(-1)
    {
        Inicializa(CelulaVazia());
    }

    void Define(size_t linha, size_t coluna, const std::string& valor)
    {
        (*this)[linha][coluna] = Formata(valor);
    }

    void Define(size_t linha, size_t coluna, double valor)
    {
        Define(linha, coluna, Formata(valor));
    }

    void DefineCabecalho(size_t linha)
    {
        linhaCabecalho = linha - 1;
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

            if (linha == linhaCabecalho)
            {
                ss << Divisoria() << std::endl;
            }
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

    std::string Divisoria() const
    {
        const size_t nCols = TamanhoLinha(_colunas, tamanhoCelulas);

        std::stringstream ss;
        for (size_t i = 0; i < nCols; i++) ss << '-';

        return ss.str();
    }

    std::string Formata(const std::string& str) const
    {
        return NormalizaCelula(str, tamanhoCelulas);
    }

    std::string Formata(double val) const
    {
        return val != FuncoesMatematicas::Sentinela() ? std::to_string(val) : "nan";
    }

    std::string NormalizaCelula(double val, size_t tam = 5u) const
    {
        return NormalizaCelula(std::to_string(val), tam);
    }

    std::string NormalizaCelula(const std::string& str, size_t tam = 7u) const
    {
        // Essa impl eh extremamente estupida, mas como eh so pra imprimir
        // a tabela e vai rodar poucas vezes, preferi deixar assim. Provavelmente
        // percorrer de tras pra frente seria muito melhor, mas essa impl ingenua
        // ja ta de bom tamanho
        std::stringstream ss;
        ss << std::right << std::setw(tam) << std::fixed << std::setprecision(tam) << str;

        const std::string strAlinhada = ss.str();
        std::string resultadoTrim;
        for (int i = 0; i < strAlinhada.length(); i++) {
            if (strAlinhada[i] != ' ')
            {
                resultadoTrim += strAlinhada[i];
            }
        }

        while (resultadoTrim.length() < tam)
        {
            std::string novoResultado = " " + resultadoTrim;
            resultadoTrim = novoResultado;
        }

        const size_t tamBuf = tam + 1;
        char* buf = new char[tamBuf];
        for (size_t i = 0; i < tam; i++) buf[i] = resultadoTrim[i];
        buf[tam] = '\0';

        std::string celula;

        celula += " ";
        celula += buf;
        celula += " ";

        delete buf;

        return celula;
    }

    constexpr size_t TamanhoLinha(size_t qtdCelulas, size_t tamanhoCelula = 5u) const
    {
        return qtdCelulas * (tamanhoCelula + 2u) + (qtdCelulas + 1u);
    }

    size_t tamanhoCelulas;
    size_t linhaCabecalho;
};

// ------------------------------------------------------------------------------------------------

#endif // TTABELA_H_
