#ifndef FUNCOESGERAIS_H_
#define FUNCOESGERAIS_H_

#include <cstdarg>
#include <iomanip>
#include <string>
#include <sstream>

// ------------------------------------------------------------------------------------------------

namespace FuncoesGerais
{
    std::string InstanciaModelo(std::string modelo, ...)
    {
        // ToDo: 1) Um mecanismo de template mais sofisticado seria interessante,
        //          em vez de so usar o simbolo de dolar (e se o usuario quiser usar o dolar?)
        //       2) Essa abordagem eh muito C-Style, talvez deveria reescrever em algo mais
        //          idiomatico em C++, como variadic templates ou parameter packs?
        std::string str;

        va_list args;
        va_start(args, modelo);

        for (size_t i = 0; i < modelo.length(); i++)
        {
            const char ch = modelo[i];
            if (ch == '$')
            {
                const char* param = va_arg(args, const char*);
                str += param;
            }
            else {
                str += ch;
            }
        }

        va_end(args);

        return str;
    }

    std::string NormalizaCelula(const std::string& str, size_t tam = 7u)
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

    std::string NormalizaCelula(double val, size_t tam = 5u)
    {
        return NormalizaCelula(std::to_string(val), tam);
    }

    constexpr size_t TamanhoLinha(size_t qtdCelulas, size_t tamanhoCelula = 5u)
    {
        return qtdCelulas * (tamanhoCelula + 2) + (qtdCelulas + 1);
    }
}

// ------------------------------------------------------------------------------------------------

#endif // FUNCOESGERAIS_H_
