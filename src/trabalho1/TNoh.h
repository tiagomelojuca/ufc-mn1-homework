#ifndef TNOH_H_
#define TNOH_H_

#include <cmath>

// ------------------------------------------------------------------------------------------------

class TNoh
{
public:
    TNoh() = default;
    TNoh(TNoh* esq, TNoh* dir) : _esq(esq), _dir(dir) {}
    virtual ~TNoh()
    {
        delete _esq;
        delete _dir;
    }

    virtual float Valor() = 0;

    TNoh* Esq()
    {
        return _esq;
    }
    void Esq(TNoh* esq)
    {
        _esq = esq;
    }
    TNoh* Dir()
    {
        return _dir;
    }
    void Dir(TNoh* dir)
    {
        _dir = dir;
    }

protected:
    TNoh* _esq = nullptr;
    TNoh* _dir = nullptr;
};

// ------------------------------------------------------------------------------------------------

class TNohOperacaoUnaria : public TNoh
{
public:
    enum class EOperacao
    {
        SIMETRICO,
        SENO,
        COSSENO,
        TANGENTE
    };

    TNohOperacaoUnaria(EOperacao operacao, TNoh* filho)
        : TNoh(filho, nullptr), _operacao(operacao) {}
    virtual ~TNohOperacaoUnaria() = default;

    float Valor() override
    {
        float valor = _esq->Valor();

        if (_operacao == EOperacao::SIMETRICO)
        {
            valor = (-1.0) * valor;
        }
        else if (_operacao == EOperacao::SENO)
        {
            valor = sin(valor);
        }
        else if (_operacao == EOperacao::COSSENO)
        {
            valor = cos(valor);
        }
        else if (_operacao == EOperacao::TANGENTE)
        {
            valor = tan(valor);
        }

        return valor;
    }

private:
    EOperacao _operacao;
};

// ------------------------------------------------------------------------------------------------

class TNohOperacaoBinaria : public TNoh
{
public:
    enum class EOperacao
    {
        SOMA,
        SUBTRACAO,
        MULTIPLICACAO,
        DIVISAO,
        EXPONENCIAL
    };

    TNohOperacaoBinaria(EOperacao operacao, TNoh* esq, TNoh* dir)
        : TNoh(esq, dir), _operacao(operacao) {}
    virtual ~TNohOperacaoBinaria() = default;

    float Valor() override
    {
        float valor = 0.0;

        float v1 = _esq->Valor();
        float v2 = _dir->Valor();

        if (_operacao == EOperacao::SOMA)
        {
            valor = v1 + v2;
        }
        else if (_operacao == EOperacao::SUBTRACAO)
        {
            valor = v1 - v2;
        }
        else if (_operacao == EOperacao::MULTIPLICACAO)
        {
            valor = v1 * v2;
        }
        else if (_operacao == EOperacao::DIVISAO)
        {
            valor = v1 / v2;
        }
        else if (_operacao == EOperacao::EXPONENCIAL)
        {
            valor = pow(v1, v2);
        }

        return valor;
    }

private:
    EOperacao _operacao;
};

// ------------------------------------------------------------------------------------------------

class TNohLiteral : public TNoh
{
public:
    TNohLiteral() = default;
    virtual ~TNohLiteral() = default;

    float Valor() override
    {
        return _chave;
    }
    void Valor(float x)
    {
        _chave = x;
    }

private:
    float _chave = 0.0;
};

// ------------------------------------------------------------------------------------------------

class TNohConstante : public TNoh
{
public:
    TNohConstante(float chave) : _chave(chave) {}

    virtual ~TNohConstante() = default;

    float Valor() override
    {
        return _chave;
    }

private:
    float _chave = 0.0;
};

// ------------------------------------------------------------------------------------------------

#endif // TNOH_H_
