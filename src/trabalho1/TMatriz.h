#ifndef TMATRIZ_H_
#define TMATRIZ_H_

// ------------------------------------------------------------------------------------------------

template <typename T>
class TMatriz
{
public:
    class TLinha // Smart Handle
    {
    public:
        TLinha(T* ptrLinha) : _ptr(ptrLinha) {}

        T& operator[](size_t coluna)
        {
            return _ptr[coluna - 1];
        }

    private:
        T* _ptr = nullptr;
    };

    TMatriz(size_t linhas, size_t colunas) : _linhas(linhas), _colunas(colunas)
    {
        _matriz = new T*[_linhas];
        for (size_t linha = 0; linha < _linhas; linha++)
        {
            _matriz[linha] = new T[_colunas];
        }
    }

    TMatriz(const TMatriz&) = delete;
    TMatriz(TMatriz&&) = delete;
    TMatriz& operator=(const TMatriz&) = delete;

    ~TMatriz()
    {
        for (size_t linha = 0; linha < _linhas; linha++)
        {
            delete[] _matriz[linha];
        }

        delete[] _matriz;
    }

    void Inicializa(const T& valorPadrao)
    {
        for (size_t linha = 0; linha < _linhas; linha++)
        {
            for (size_t coluna = 0; coluna < _colunas; coluna++)
            {
                _matriz[linha][coluna] = valorPadrao;
            }
        }
    }

    TLinha operator[](size_t linha)
    {
        return TLinha(_matriz[linha - 1]);
    }

protected:
    size_t _linhas;
    size_t _colunas;

    T** _matriz;
};

// ------------------------------------------------------------------------------------------------

#endif // TMATRIZ_H_
