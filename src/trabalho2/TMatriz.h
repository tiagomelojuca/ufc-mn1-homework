#ifndef TMATRIZ_H_
#define TMATRIZ_H_

#include <cmath>
#include <cstdint>
#include <vector>

// ------------------------------------------------------------------------------------------------

class TMatriz
{
public:
    TMatriz() = delete;

    TMatriz(const TMatriz& outra)
    {
        _linhas = outra._linhas;
        _colunas = outra._colunas;
        AlocaMem(_linhas, _colunas);

        for (uint8_t linha = 0; linha < _linhas; linha++)
        {
            for (uint8_t coluna = 0; coluna < _colunas; coluna++)
            {
                _matrizCrua[linha][coluna] = outra._matrizCrua[linha][coluna];
            }
        }
    }

    TMatriz(TMatriz&& outra)
    {
        _linhas = outra._linhas;
        outra._linhas = 0;

        _colunas = outra._colunas;
        outra._colunas = 0;

        _matrizCrua = outra._matrizCrua;
        outra._matrizCrua = nullptr;
    }

    TMatriz(uint8_t linhas, uint8_t colunas)
    {
        _linhas = linhas;
        _colunas = colunas;
        AlocaMem(_linhas, _colunas);
    }

    TMatriz(uint8_t linhas, uint8_t colunas, const std::vector<std::vector<double>>& elementos)
    {
        _linhas = linhas;
        _colunas = colunas;
        AlocaMem(_linhas, _colunas);

        for (uint8_t linha = 0; linha < _linhas; linha++)
        {
            for (uint8_t coluna = 0; coluna < _colunas; coluna++)
            {
                _matrizCrua[linha][coluna] = elementos[linha][coluna];
            }
        }
    }

    ~TMatriz()
    {
        LimpaMem();
    }

    TMatriz& operator=(const TMatriz& outra)
    {
        if (&outra != this)
        {
            LimpaMem();

            _linhas = outra._linhas;
            _colunas = outra._colunas;
            AlocaMem(_linhas, _colunas);

            for (uint8_t linha = 0; linha < _linhas; linha++)
            {
                for (uint8_t coluna = 0; coluna < _colunas; coluna++)
                {
                    _matrizCrua[linha][coluna] = outra._matrizCrua[linha][coluna];
                }
            }
        }

        return *this;
    }

    TMatriz& operator=(TMatriz&& outra)
    {
        if (&outra != this)
        {
            _linhas = outra._linhas;
            outra._linhas = 0;

            _colunas = outra._colunas;
            outra._colunas = 0;

            if (_matrizCrua != nullptr)
            {
                LimpaMem();
            }
            _matrizCrua = outra._matrizCrua;
            outra._matrizCrua = nullptr;
        }

        return *this;
    }

    bool operator==(const TMatriz& outra) const
    {
        return Igual(outra, 1e-4f);
    }

    uint8_t Linhas() const
    {
        return _linhas;
    }
    uint8_t Colunas() const
    {
        return _colunas;
    }

    double Valor(uint8_t l, uint8_t c) const
    {
        return _matrizCrua[l - 1][c - 1];
    }

    bool Inconsistente() const
    {
        return _linhas == 0 || _colunas == 0;
    }

    bool Igual(const TMatriz& outra, float tol) const
    {
        if (_linhas != outra._linhas || _colunas != outra._colunas)
        {
            return false;
        }

        for (int l = 1; l <= _linhas; l++)
        {
            for (int c = 1; c <= _colunas; c++)
            {
                if (fabs(Valor(l, c) - outra.Valor(l, c)) > tol)
                {
                    return false;
                }
            }
        }

        return true;
    }

    TMatriz Inversa() const
    {
        // ToDo
        return TMatriz(0, 0);
    }

private:
    void LimpaMem()
    {
        for (uint8_t linha = 0; linha < _linhas; linha++)
        {
            delete[] _matrizCrua[linha];
        }

        delete[] _matrizCrua;
    }
    void AlocaMem(uint8_t linhas, uint8_t colunas)
    {
        _matrizCrua = new double*[linhas];
        for (uint8_t linha = 0; linha < linhas; linha++)
        {
            _matrizCrua[linha] = new double[colunas]{};
        }
    }

    uint8_t _linhas  = 0u;
    uint8_t _colunas = 0u;

    double** _matrizCrua = nullptr;
};

// ------------------------------------------------------------------------------------------------

#endif // TMATRIZ_H_
