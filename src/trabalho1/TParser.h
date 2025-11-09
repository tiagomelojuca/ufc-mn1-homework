#ifndef TPARSER_H_
#define TPARSER_H_

#include <algorithm>
#include <vector>

#include "IParser.h"
#include "TArvoreSintatica.h"

// ------------------------------------------------------------------------------------------------

class TLexer
{
public:
    constexpr static const char* CARACTERES_ALFABETICOS_MINUSCULOS = "abcdefghijklmnopqrstuvwxyz";
    constexpr static const char* CARACTERES_ALFABETICOS_MAIUSCULOS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr static const char* CARACTERES_NUMERICOS = "0123456789.,";
    constexpr static const char* CARACTERES_SIMBOLICOS = "+-*/^()=";

    static std::vector<std::string> PalavrasReservadas()
    {
        std::vector<std::string> palavrasReservadas;

        palavrasReservadas.push_back("sin");
        palavrasReservadas.push_back("sen");
        palavrasReservadas.push_back("cos");
        palavrasReservadas.push_back("tan");
        palavrasReservadas.push_back("tg");
        palavrasReservadas.push_back("pi");

        return palavrasReservadas;
    }

    static std::string Normaliza(const std::string& palavraReservada)
    {
        if (palavraReservada == "sen") return "sin";
        if (palavraReservada == "tg")  return "tan";

        return palavraReservada;
    }

    static bool EhPalavraReservada(const std::string& palavra)
    {
        for (const std::string& palavraReservada : PalavrasReservadas())
        {
            if (palavra == palavraReservada)
            {
                return true;
            }
        }

        return false;
    }

    enum class EToken
    {
        NUMERO, IDENTIFICADOR, OPERADOR, ABERTURA_SUBEXPR, FECHAMENTO_SUBEXPR, IGUAL, FIM
    };
    static const EToken TOKEN_INVALIDO = static_cast<EToken>(-1);

    class TToken {
    public:
        TToken() = default;

        TToken(EToken tipoToken, const std::string& conteudo, size_t posicao)
            : _tipo(tipoToken), _valor(conteudo), _posicao(posicao) {}

        bool Valido() const
        {
            return _tipo != TOKEN_INVALIDO;
        }

        EToken Tipo() const
        {
            return _tipo;
        }

        const std::string& Valor() const
        {
            return _valor;
        }

        size_t Posicao() const
        {
            return _posicao;
        }

        std::string ToString() const
        {
            std::string str;

            str += "{ ";
            str += "EToken::";

            if (_tipo == EToken::NUMERO) {
                str += "NUMERO";
            }
            else if (_tipo == EToken::IDENTIFICADOR) {
                str += "IDENTIFICADOR";
            }
            else if (_tipo == EToken::OPERADOR) {
                str += "OPERADOR";
            }
            else if (_tipo == EToken::ABERTURA_SUBEXPR) {
                str += "ABERTURA_SUBEXPR";
            }
            else if (_tipo == EToken::FECHAMENTO_SUBEXPR) {
                str += "FECHAMENTO_SUBEXPR";
            }
            else if (_tipo == EToken::IGUAL) {
                str += "IGUAL";
            }
            else if (_tipo == EToken::FIM) {
                str += "FIM";
            }
            else {
                str += "TOKEN_INVALIDO";
            }

            str += ", \"";
            str += _valor;
            str += "\", ";
            str += std::to_string(_posicao);
            str += " }";

            return str;
        }

    private:
        EToken _tipo = TOKEN_INVALIDO;
        std::string _valor;
        size_t _posicao = 0;
    };

    TLexer() = default;
    TLexer(const std::string& expr) : _expr(expr) {}

    TToken Proximo()
    {
        return Normalizado(ProcessaProximo());
    }

    void Solicita(const TToken& t)
    {
        Solicita(t.Posicao());
    }

private:
    TToken Normalizado(const TToken& token) const
    {
        if (token.Tipo() == EToken::NUMERO)
        {
            std::string numero = token.Valor();
            for (size_t i = 0; i < numero.length(); i++)
            {
                if (numero[i] == ',')
                {
                    numero[i] = '.';
                }
            }

            return { token.Tipo(), numero, token.Posicao() };
        }

        if (token.Tipo() == EToken::IDENTIFICADOR)
        {
            std::string identificador = token.Valor();
            for (size_t i = 0; i < identificador.length(); i++)
            {
                identificador[i] = ToLower(identificador[i]);
            }

            return { token.Tipo(), Normaliza(identificador), token.Posicao() };
        }

        return token;
    }

    TToken ProcessaProximo()
    {
        AvancaEspacos();

        const char chCorrente = Corrente();
        if (EhCaractereFimCadeia(chCorrente))  return ProcessaFim();
        if (EhCaractereNumerico(chCorrente))   return ProcessaNumero();
        if (EhCaractereAlfabetico(chCorrente)) return ProcessaIdentificador();
        if (EhCaractereSimbolico(chCorrente))  return ProcessaSimbolo();

        return TToken {};
    }

    TToken ProcessaFim()
    {
        return { EToken::FIM, "", _posicao };
    }

    TToken ProcessaNumero()
    {
        const size_t posicao = _posicao;

        std::string numero;
        while (EhCaractereNumerico(Corrente()))
        {
            numero += Corrente();
            Avanca();
        }

        return TToken { EToken::NUMERO, numero, posicao };
    }

    TToken ProcessaIdentificador()
    {
        for (const std::string& palavraReservada : PalavrasReservadas())
        {
            const TToken token = ProcessaPalavra(palavraReservada);
            if (token.Valido()) {
                return token;
            }
        }

        const TToken token { EToken::IDENTIFICADOR, ToString(Corrente()), _posicao };
        Avanca();
        return token;
    }

    TToken ProcessaPalavra(const std::string& palavra)
    {
        bool ok = true;

        for (size_t i = 0; i < palavra.length(); i++)
        {
            const char ch = Espia(i);
            const char chLower = ToLower(ch);
            const char chUpper = ToUpper(ch);

            if (palavra[i] != chLower && palavra[i] != chUpper)
            {
                ok = false;
                break;
            }
        }

        if (!ok)
        {
            return TToken {};
        }

        const size_t posicao = _posicao;

        for (size_t i = 0; i < palavra.length(); i++)
        {
            Avanca();
        }

        return { EToken::IDENTIFICADOR, palavra, posicao };
    }

    TToken ProcessaSimbolo()
    {
        const char ch = Corrente();

        EToken tipoToken = EToken::OPERADOR;
        if (ch == '(')
        {
            tipoToken = EToken::ABERTURA_SUBEXPR;
        }
        else if (ch == ')')
        {
            tipoToken = EToken::FECHAMENTO_SUBEXPR;
        }
        else if (ch == '=')
        {
            tipoToken = EToken::IGUAL;
        }

        const TToken token { tipoToken, ToString(ch), _posicao };
        Avanca();
        return token;
    }

    void AvancaEspacos()
    {
        while (Corrente() == ' ')
        {
            Avanca();
        }
    }

    void Avanca()
    {
        if (_posicao < _expr.length())
        {
            _posicao++;
        }
    }

    char Consome()
    {
        const char ch = Corrente();
        Avanca();
        return ch;
    }

    char Anterior() const { return Caractere(_posicao - 1); }
    char Corrente() const { return Caractere(_posicao); }
    char Seguinte() const { return Caractere(_posicao + 1); }

    char Espia(size_t offset) const // Peek
    {
        return Caractere(_posicao + offset);
    }
    void Solicita(size_t pos) // Seek
    {
        if (pos < _expr.length())
        {
            _posicao = pos;
        }
    }

    char Caractere(size_t pos) const
    {
        return pos < _expr.length() ? _expr[pos] : '\0';
    }

    bool EhCaractereFimCadeia(char ch) const
    {
        return ch == '\0';
    }
    bool EhCaractereAlfabetico(char ch) const
    {
        return StringContemCaractere(CARACTERES_ALFABETICOS_MINUSCULOS, ch) ||
               StringContemCaractere(CARACTERES_ALFABETICOS_MAIUSCULOS, ch);
    }
    bool EhCaractereNumerico(char ch) const
    {
        return StringContemCaractere(CARACTERES_NUMERICOS, ch);
    }
    bool EhCaractereSimbolico(char ch) const
    {
        return StringContemCaractere(CARACTERES_SIMBOLICOS, ch);
    }
    bool StringContemCaractere(const char* str, char ch) const
    {
        for (size_t i = 0; str[i] != '\0'; i++)
        {
            if (ch == str[i])
            {
                return true;
            }
        }

        return false;
    }

    char ToLower(char ch) const
    {
        return Transforma(
            ch, CARACTERES_ALFABETICOS_MAIUSCULOS, CARACTERES_ALFABETICOS_MINUSCULOS
        );
    }

    char ToUpper(char ch) const
    {
        return Transforma(
            ch, CARACTERES_ALFABETICOS_MINUSCULOS, CARACTERES_ALFABETICOS_MAIUSCULOS
        );
    }

    char Transforma(char ch, const char* de, const char* para) const
    {
        for (size_t i = 0; de[i] != '\0'; i++)
        {
            if (ch == de[i])
            {
                return para[i];
            }
        }

        return ch;
    }

    std::string ToString(char ch) const
    {
        std::string str;
        str += ch;
        return str;
    }

    std::string _expr;
    size_t _posicao = 0;
};

// ------------------------------------------------------------------------------------------------

class TParser : public IParser
{
public:
    TParser() = default;
    virtual ~TParser() = default;

    TNoh* Parse(const std::string& expr) override
    {
        _lexer = TLexer(expr);

        if (!ExpressaoValida())
        {
            return nullptr;
        }
        
        return new TNohConstante { 0.0 };
    }

private:
    bool ExpressaoValida()
    {
        return CabecalhoValido() && CorpoValido();
    }

    bool CabecalhoValido()
    {
        std::vector<TLexer::EToken> tokensEsperados;
        tokensEsperados.push_back(TLexer::EToken::IDENTIFICADOR);      // f
        tokensEsperados.push_back(TLexer::EToken::ABERTURA_SUBEXPR);   // (
        tokensEsperados.push_back(TLexer::EToken::IDENTIFICADOR);      // x
        tokensEsperados.push_back(TLexer::EToken::FECHAMENTO_SUBEXPR); // )
        tokensEsperados.push_back(TLexer::EToken::IGUAL);              // =

        std::vector<TLexer::TToken> tokensLidos;

        bool cabecalhoValido = true;
        for (size_t i = 0; i < 5; i++)
        {
            const TLexer::TToken token = _lexer.Proximo();
            tokensLidos.push_back(token);

            if (!token.Valido() || token.Tipo() != tokensEsperados[i])
            {
                cabecalhoValido = false;
                break;
            }
        }

        if (cabecalhoValido)
        {
            const std::string variavelDependente = tokensLidos[0].Valor();
            const std::string variavelIndependente = tokensLidos[2].Valor();

            cabecalhoValido = variavelDependente != variavelIndependente &&
                              !TLexer::EhPalavraReservada(variavelDependente) &&
                              !TLexer::EhPalavraReservada(variavelIndependente);

            if (cabecalhoValido)
            {
                _variavelDependente = variavelDependente;
                _variavelIndependente = variavelIndependente;
            }
        }

        return cabecalhoValido;
    }

    bool CorpoValido()
    {
        return !TemTokenInvalido() && !ParentesisInvalidos();
    }

    bool TemTokenInvalido()
    {
        bool temTokenInvalido = false;

        const TLexer::TToken inicioCorpo = _lexer.Proximo();

        TLexer::TToken tokenCorrente = inicioCorpo;
        while (tokenCorrente.Tipo() != TLexer::EToken::FIM)
        {
            if (!TokenValido(tokenCorrente))
            {
                temTokenInvalido = true;
                break;
            }

            tokenCorrente = _lexer.Proximo();
        }

        _lexer.Solicita(inicioCorpo);

        return temTokenInvalido;
    }

    bool ParentesisInvalidos()
    {
        bool temParentesisInvalidos = false;

        const TLexer::TToken inicioCorpo = _lexer.Proximo();

        std::vector<TLexer::TToken> aberturas;
        std::vector<TLexer::TToken> fechamentos;

        TLexer::TToken tokenAnterior;
        TLexer::TToken tokenCorrente = inicioCorpo;
        while (tokenCorrente.Tipo() != TLexer::EToken::FIM)
        {
            if (tokenCorrente.Tipo() == TLexer::EToken::ABERTURA_SUBEXPR)
            {
                aberturas.push_back(tokenCorrente);
            }
            else if (tokenCorrente.Tipo() == TLexer::EToken::FECHAMENTO_SUBEXPR)
            {
                if (tokenAnterior.Tipo() == TLexer::EToken::ABERTURA_SUBEXPR)
                {
                    temParentesisInvalidos = true;
                    break;
                }

                fechamentos.push_back(tokenCorrente);
            }

            tokenAnterior = tokenCorrente;
            tokenCorrente = _lexer.Proximo();
        }

        temParentesisInvalidos |= aberturas.size() != fechamentos.size();

        if (!temParentesisInvalidos)
        {
            std::sort(aberturas.begin(), aberturas.end(), [](const auto& t1, const auto& t2)
            {
                return t1.Posicao() < t2.Posicao();
            });
            std::sort(fechamentos.begin(), fechamentos.end(), [](const auto& t1, const auto& t2)
            {
                return t1.Posicao() < t2.Posicao();
            });

            for (int i = aberturas.size() - 1; i >= 0; i--)
            {
                for (int j = 0; j < fechamentos.size(); j++)
                {
                    if (fechamentos[j].Posicao() > aberturas[i].Posicao())
                    {
                        fechamentos.erase(fechamentos.begin() + j);
                        break;
                    }
                }
            }

            temParentesisInvalidos = fechamentos.size() != 0;
        }

        _lexer.Solicita(inicioCorpo);

        return temParentesisInvalidos;
    }

    bool TokenValido(const TLexer::TToken& token) const
    {
        if (token.Tipo() == TLexer::EToken::IDENTIFICADOR)
        {
            return token.Valor() == _variavelIndependente ||
                   TLexer::EhPalavraReservada(token.Valor());
        }

        return token.Tipo() != TLexer::EToken::IGUAL;
    }

    TLexer _lexer;

    std::string _variavelDependente;
    std::string _variavelIndependente;
};

// ------------------------------------------------------------------------------------------------

#endif // TPARSER_H_
