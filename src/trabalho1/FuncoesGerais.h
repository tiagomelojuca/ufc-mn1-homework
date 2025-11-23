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
}

// ------------------------------------------------------------------------------------------------

#endif // FUNCOESGERAIS_H_
