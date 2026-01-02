#ifndef TMATRIZ_H_
#define TMATRIZ_H_

#include <cstdint>
#include <vector>

// ------------------------------------------------------------------------------------------------

class TMatriz
{
public:
    TMatriz() = delete;

    TMatriz(const TMatriz&)
    {
        // ToDo
    }

    TMatriz(TMatriz&&)
    {
        // ToDo
    }

    TMatriz(uint8_t linhas, uint8_t colunas)
    {
        // ToDo
    }

    TMatriz(uint8_t linhas, uint8_t colunas, const std::vector<std::vector<float>>& elementos)
    {
        // ToDo
    }

    bool operator==(const TMatriz& outra) const
    {
        return Igual(outra, 0.0f);
    }

    TMatriz Inversa() const
    {
        // ToDo
        return TMatriz(0, 0);
    }

    bool Igual(const TMatriz& outra, float tol) const
    {
        return false;
    }

private:
    uint8_t _linhas  = 0u;
    uint8_t _colunas = 0u;
};

// ------------------------------------------------------------------------------------------------

#endif // TMATRIZ_H_
