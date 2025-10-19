#ifndef ABB_H_
#define ABB_H_

#include <functional>
#include <string>

#define _MAXINT 2147483647

namespace ufc
{
namespace mn1
{
namespace trabalho1
{

class abb
{
public:
    class noh
    {
    public:
        int chave() const
        {
            return _chave;
        }
        void chave(int n)
        {
            _chave = n;
        }

        const noh* pai() const
        {
            return _pai;
        }
        noh* pai()
        {
            return _pai;
        }
        void pai(noh* n)
        {
            _pai = n;
        }

        noh* esq()
        {
            return _esq;
        }
        noh* esq() const
        {
            return _esq;
        }
        void esq(noh* n)
        {
            _esq = n;
        }

        noh* dir()
        {
            return _dir;
        }
        noh* dir() const
        {
            return _dir;
        }
        void dir(noh* n)
        {
            _dir = n;
        }

    private:
        int _chave = 0;
        noh* _pai = nullptr;
        noh* _esq = nullptr;
        noh* _dir = nullptr;
    };

    ~abb()
    {
        visita_pos_ordem([](const noh* x) {
            delete x;
        });
    }

    bool inclui(int chave)
    {
        auto z = new noh;
        z->chave(chave);
        inclui(z);

        return true;
    }

    bool remove(int chave)
    {
        noh* z = busca(raiz, chave);
        if (z == nullptr)
        {
            return false;
        }

        remove(z);
        delete z;

        return true;
    }

    int sucessor(int x) const
    {
        noh* n = busca_menor_igual(x);
        if (n != nullptr)
        {
            n = sucessor(n);

            if (n != nullptr)
            {
                return n->chave();
            }
        }

        return _MAXINT;
    }

    std::string to_string() const
    {
        std::string str;

        visita_em_ordem([&str](const noh* x) {
            str += std::to_string(x->chave());
            str += " ";
        });

        if (!str.empty())
        {
            str.pop_back();
        }

        return str;
    }

    // Metodos para depuracao
    std::string _arvore_em_ascii() const
    {
        std::string arvore;
        _arvore_em_ascii(arvore, raiz, "", true);

        return arvore;
    }

private:
    // Referencias:
    // https://www.youtube.com/watch?v=f7sIuYI5M2Y
    // https://www.youtube.com/watch?v=QA2wFn9nQU4

    void visita_em_ordem(std::function<void(const noh*)> visita) const
    {
        visita_em_ordem(raiz, visita);
    }

    void visita_em_ordem(const noh* x, std::function<void(const noh*)> visita) const
    {
        if (x != nullptr)
        {
            visita_em_ordem(x->esq(), visita);
            visita(x);
            visita_em_ordem(x->dir(), visita);
        }
    }

    void visita_pos_ordem(std::function<void(const noh*)> visita) const
    {
        visita_pos_ordem(raiz, visita);
    }

    void visita_pos_ordem(const noh* x, std::function<void(const noh*)> visita) const
    {
        if (x != nullptr)
        {
            visita_pos_ordem(x->esq(), visita);
            visita_pos_ordem(x->dir(), visita);
            visita(x);
        }
    }

    noh* busca(noh* x, int chave) const
    {
        while (x != nullptr && x->chave() != chave)
        {
            x = chave < x->chave() ? x->esq() : x->dir();
        }

        return x;
    }

    noh* busca_menor_igual(int chave) const
    {
        noh* x = raiz;
        noh* y = nullptr;
        while (x != nullptr && x->chave() != chave)
        {
            y = x;
            x = chave < x->chave() ? x->esq() : x->dir();
        }

        return x != nullptr ? x : y;
    }

    noh* min(noh* x) const
    {
        while (x->esq() != nullptr)
        {
            x = x->esq();
        }

        return x;
    }

    noh* sucessor(noh* x) const
    {
        if (x->dir() != nullptr && x->dir()->chave() != x->chave())
        {
            return min(x->dir());
        }

        noh* y = x->pai();
        while (y != nullptr && x == y->dir())
        {
            x = y;
            y = x->pai();
        }

        return y;
    }

    void inclui(noh* z)
    {
        noh* y = nullptr;
        noh* x = raiz;

        while (x != nullptr)
        {
            y = x;
            x = z->chave() < x->chave() ? x->esq() : x->dir();
        }

        z->pai(y);
        if (y == nullptr)
        {
            raiz = z;
        }
        else if (z->chave() < y->chave()) {
            y->esq(z);
        }
        else {
            y->dir(z);
        }
    }

    void remove(noh* z)
    {
        if (z->esq() == nullptr)
        {
            transplanta(z, z->dir());
        }
        else if (z->dir() == nullptr)
        {
            transplanta(z, z->esq());
        }
        else
        {
            noh* y = sucessor(z);
            transplanta(y, y->dir());

            y->esq(z->esq());
            z->esq()->pai(y);
            y->dir(z->dir());

            if (z->dir() != nullptr)
            {
                z->dir()->pai(y);
            }
            transplanta(z, y);
        }
    }

    void transplanta(noh* u, noh* v)
    {
        if (u == nullptr)
        {
            return;
        }

        if (u->pai() == nullptr)
        {
            raiz = v;
        }
        else if (u == u->pai()->esq())
        {
            u->pai()->esq(v);
        }
        else
        {
            u->pai()->dir(v);
        }

        if (v != nullptr)
        {
            v->pai(u->pai());
        }
    }

    void _arvore_em_ascii(std::string& out, const noh* x, const std::string& prefixo, bool ehUltimo) const
    {
        if (x != nullptr)
        {
            out += prefixo;
            out += (ehUltimo ? "|___" : "|---");
            out += std::to_string(x->chave());
            out += "\n";

            const std::string novoPrefixo = prefixo + (ehUltimo ? "    " : "|   ");
            if (x->esq() != nullptr || x->dir() != nullptr)
            {
                if (x->dir() != nullptr)
                {
                    _arvore_em_ascii(out, x->dir(), novoPrefixo, x->esq() == nullptr);
                }

                if (x->esq() != nullptr)
                {
                    _arvore_em_ascii(out, x->esq(), novoPrefixo, true);
                }
            }
        }
    }

    noh* raiz = nullptr;
};

}
}
}

#endif // ABB_H_
