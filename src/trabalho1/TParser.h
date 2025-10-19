#ifndef TPARSER_H_
#define TPARSER_H_

#include <algorithm>
#include <string>

#include "TArvoreSintatica.h"

#include <iostream>

// ------------------------------------------------------------------------------------------------

class TParser
{
private:
    struct TOcorrenciaCaractere { char ch; size_t pos; };

    struct TToken
    {
        std::string token;

        TToken* anterior;
        TToken* seguinte;
    };

public:
    TParser() = default;

    TNoh* Parse(std::string expr)
    {
        TNoh* noh = nullptr;

        Normaliza(expr);
        if (ExpressaoValida(expr))
        {
            noh = new TNohLiteral;

            TToken* tokenCorrente = nullptr;

            std::string numeroCorrente = "";

            for (int i = 0; i < expr.length(); i++)
            {
                const char ch = expr[i];
                if (EhCaractereNumerico(ch))
                {
                    numeroCorrente += ch;
                }
                else
                {
                    if (numeroCorrente != "")
                    {
                        auto novoToken = new TToken { numeroCorrente, tokenCorrente, nullptr };
                        if (tokenCorrente)
                        {
                            tokenCorrente->seguinte = novoToken;
                        }
                        tokenCorrente = novoToken;

                        numeroCorrente = "";
                    }

                    std::string strToken;
                    strToken += ch;

                    auto novoToken = new TToken { strToken, tokenCorrente, nullptr };
                    if (tokenCorrente)
                    {
                        tokenCorrente->seguinte = novoToken;
                    }
                    tokenCorrente = novoToken;
                }
            }

            if (numeroCorrente != "")
            {
                auto novoToken = new TToken { numeroCorrente, tokenCorrente, nullptr };
                if (tokenCorrente)
                {
                    tokenCorrente->seguinte = novoToken;
                }
                tokenCorrente = novoToken;

                numeroCorrente = "";
            }

            TToken* seguinte = nullptr;
            while (tokenCorrente)
            {
                seguinte = tokenCorrente;
                tokenCorrente = tokenCorrente->anterior;
            }
            tokenCorrente = seguinte;

            std::cout << "tokens: ";
            while (tokenCorrente)
            {
                std::cout << "\"" << tokenCorrente->token << "\" ";
                tokenCorrente = tokenCorrente->seguinte;
            }
            std::cout << "\n";
        }

        return noh;
    }
    
private:
    void Normaliza(std::string& str) const
    {
        RemoveEspacos(str);

        Substituir(str, "f(x)=", "");
        Substituir(str, "sen", "s");
        Substituir(str, "sin", "s");
        Substituir(str, "cos", "c");
        Substituir(str, "tan", "t");
        Substituir(str, "tg", "t");

        RemoveDuplicatas(str, '.');
        RemoveDuplicatas(str, '+');
        RemoveDuplicatas(str, '-');
        RemoveDuplicatas(str, '/');
        RemoveDuplicatas(str, '*');
        RemoveDuplicatas(str, '^');
        RemoveDuplicatas(str, 's');
        RemoveDuplicatas(str, 'c');
        RemoveDuplicatas(str, 't');
        RemoveDuplicatas(str, 'x');

        RemoveParentesisInuteis(str);
    }

    bool ExpressaoValida(const std::string& str) const
    {
        return str != "" && !TemCaractereInvalido(str) && TemParidadeParentesis(str);
    }

    bool TemCaractereInvalido(const std::string& str) const
    {
        for (int i = 0; i < str.length(); i++)
        {
            if (!EhCaractereValido(str[i]))
            {
                return true;
            }
        }

        return false;
    }

    bool TemParidadeParentesis(const std::string& str) const
    {
        std::vector<TOcorrenciaCaractere> ocorrencias = MapeiaOcorrenciasParentesis(str);
        std::vector<TOcorrenciaCaractere> aberturas;
        std::vector<TOcorrenciaCaractere> fechamentos;

        for (const TOcorrenciaCaractere& ocorrencia : ocorrencias) {
            if (ocorrencia.ch == '(')
            {
                aberturas.push_back(ocorrencia);
            }
            else
            {
                fechamentos.push_back(ocorrencia);
            }
        }

        bool paridadeOk = aberturas.size() == fechamentos.size();

        if (paridadeOk)
        {
            std::sort(aberturas.begin(), aberturas.end(), [](const auto& o1, const auto& o2)
            {
                return o1.pos < o2.pos;
            });
            std::sort(fechamentos.begin(), fechamentos.end(), [](const auto& o1, const auto& o2)
            {
                return o1.pos < o2.pos;
            });

            for (int i = aberturas.size() - 1; i >= 0; i--)
            {
                for (int j = 0; j < fechamentos.size(); j++)
                {
                    if (fechamentos[j].pos > aberturas[i].pos)
                    {
                        fechamentos.erase(fechamentos.begin() + j);
                        break;
                    }
                }
            }

            paridadeOk = fechamentos.size() == 0;
        }

        return paridadeOk;
    }

    bool EhCaractereValido(char ch) const
    {
        constexpr const char* caracteresValidos = "fxsct()+-*/^.0123456789";

        for (int i = 0; caracteresValidos[i] != '\0'; i++)
        {
            if (ch == caracteresValidos[i])
            {
                return true;
            }
        }

        return false;
    }
    bool EhCaractereNumerico(char ch) const
    {
        constexpr const char* caracteresValidos = ".0123456789";

        for (int i = 0; caracteresValidos[i] != '\0'; i++)
        {
            if (ch == caracteresValidos[i])
            {
                return true;
            }
        }

        return false;
    }

    std::vector<TOcorrenciaCaractere>
    MapeiaOcorrenciasParentesis(const std::string& str) const
    {
        std::vector<TOcorrenciaCaractere> ocorrencias;

        for (size_t i = 0; i < str.length(); i++)
        {
            if (str[i] == '(' || str[i] == ')')
            {
                ocorrencias.push_back({ str[i], i });
            }
        }

        return ocorrencias;
    }

    void RemoveEspacos(std::string& str) const
    {
        std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
        str.erase(end_pos, str.end());
    }

    void RemoveParentesisInuteis(std::string& str) const
    {
        while (Substituir(str, "()", ""));
    }

    void RemoveDuplicatas(std::string& str, char ch) const
    {
        std::string duplicata;
        duplicata += ch;
        duplicata += ch;

        while (Substituir(str, duplicata, ""));
    }

    bool Substituir(std::string& str, const std::string& de, const std::string& para) const
    {
        bool substituiu = false;

        size_t pos_inicio = 0;

        while((pos_inicio = str.find(de, pos_inicio)) != std::string::npos)
        {
            str.replace(pos_inicio, de.length(), para);
            pos_inicio += para.length();

            substituiu = true;
        }

        return substituiu;
    }

    std::string _expr;
};

// ------------------------------------------------------------------------------------------------

#endif
