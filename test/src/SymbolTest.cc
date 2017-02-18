#include <gtest/gtest.h>

#include "Symbol.h"

class SymbolTest : public ::testing::Test {
    protected:
        Character *A, *B, *C;

        void SetUp() {
            A = new Character('A');
            B = new Character('B');
            C = new Character('C');
        }

        void TearDown() {
            delete A;
            delete B;
            delete C;
        }
};

TEST_F(SymbolTest, StringLength) {
    SymbolString string;
    EXPECT_EQ(0, string.length());
    
    string.append(A);
    string.append(B);
    string.append(C);
    EXPECT_EQ(3, string.length());
}

TEST_F(SymbolTest, StringReverse) {
    SymbolString string;
    string.append(A);
    string.append(B);
    string.append(C);

    SymbolString exp;
    exp.append(C);
    exp.append(B);
    exp.append(A);

    SymbolString *rev = string.reversed();
    EXPECT_TRUE(exp.equals(*rev));
    delete rev;
}

TEST_F(SymbolTest, StringTruncate) {
    SymbolString string;
    string.append(A);
    string.append(B);
    string.append(C);

    SymbolString exp;
    exp.append(A);
    exp.append(B);

    SymbolString *trunc = string.truncated(1);
    EXPECT_TRUE(exp.equals(*trunc));
    delete trunc;
}

TEST_F(SymbolTest, SetDifference) {
    SymbolSet set;
    set.add(A);
    set.add(B);
    set.add(C);

    SymbolSet other;
    other.add(B);

    SymbolSet exp;
    exp.add(A);
    exp.add(C);

    SymbolSet *out = set.difference(&other);
    
    EXPECT_EQ(exp, *out);
    delete out;
}
