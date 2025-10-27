#include <iostream>

// bibliotecas utilizadas: apenas a biblioteca padrao do C++,
// para lidar com E/S, strings seguras, e conversoes num <-> str

int pot(int n, int exp)
{
    if (exp <= 0)
    {
        return 1;
    }
    
    int acc = n;
    for (int i = 2; i <= exp; i++)
    {
        acc *= n;
    }
    
    return acc;
}

std::string bin2dec(const std::string& bin)
{
    int dec = 0;
    
    int exp = 0;
    for (int i = bin.length() - 1; i >= 0; i--, exp++)
    {
        if (bin[i] == '1')
        {
            dec += pot(2, exp);
        }
    }

    return std::to_string(dec);
}

std::string dec2bin(const std::string& dec)
{
    std::string _bin;
    
    int _dec = std::atoi(dec.c_str());
    
    if (_dec <= 0)
    {
        return "0";
    }
    
    while (_dec != 0)
    {
        _bin += std::to_string(_dec % 2);
        _dec /= 2;
    }
    
    const std::string _bkp = _bin;
    _bin = "";
    for (int i = _bkp.length() - 1; i >= 0; i--)
    {
        _bin += _bkp[i];
    }

    return _bin;
}

int main()
{
    std::cout << "dec2bin(27)    = " << dec2bin("27") << std::endl;
    std::cout << "bin2dec(11011) = " << bin2dec("11011") << std::endl;
}
