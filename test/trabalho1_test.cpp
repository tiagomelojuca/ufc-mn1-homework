#include <gtest/gtest.h>

#include "trabalho1/abb.h"

TEST(trabalho1_test, deve_ser_capaz_de_realizar_operacoes_basicas_em_arvore)
{
    ufc::mn1::trabalho1::abb arvore;

    arvore.inclui(6);
    arvore.inclui(5);
    arvore.inclui(7);
    arvore.inclui(8);
    arvore.inclui(5);
    arvore.inclui(2);

    arvore.inclui(4);
    arvore.inclui(4);
    arvore.remove(4);
    arvore.remove(4);

    EXPECT_STREQ(arvore.to_string().c_str(), "2 5 5 6 7 8");

    EXPECT_EQ(arvore.sucessor(2), 5);
    EXPECT_EQ(arvore.sucessor(5), 6);
    EXPECT_EQ(arvore.sucessor(6), 7);
    EXPECT_EQ(arvore.sucessor(7), 8);
    EXPECT_EQ(arvore.sucessor(8), _MAXINT);
    EXPECT_EQ(arvore.sucessor(3), 5);
    EXPECT_EQ(arvore.sucessor(4), 5);
}
