#include <gtest/gtest.h>

#include "State.h"

class StateTest : public ::testing::Test {
    protected:
        State *s, *p, *q, *r;

        void SetUp() {
            s = new SimpleState(1);
            p = new SimpleState(2);
            q = new SimpleState(1);
            r = new SimpleState(3);
        }

        virtual void TearDown() {
            delete s;
            delete p;
            delete q;
            delete r;
        }
};

TEST_F(StateTest, PairStateEquality) {
    PairState pair_1(s, p);
    PairState pair_2(p, s);
    PairState pair_3(q, p);

    EXPECT_FALSE(pair_1.equals(pair_2));
    EXPECT_TRUE(pair_1.equals(pair_3));
}

TEST_F(StateTest, SetRemoveAll) {
    StateSet set;
    set.add(s);
    set.add(p);
    set.add(r);

    StateSet other;
    other.add(q);
    other.add(r);

    set.remove_all(&other);

    EXPECT_TRUE(set.contains(p));
    EXPECT_FALSE(set.contains(s));
    EXPECT_FALSE(set.contains(r));
}

TEST_F(StateTest, SetRetainAll) {
    StateSet set;
    set.add(s);
    set.add(p);
    set.add(r);

    StateSet other;
    other.add(q);
    other.add(r);

    set.retain_all(&other);

    EXPECT_FALSE(set.contains(p));
    EXPECT_TRUE(set.contains(s));
    EXPECT_TRUE(set.contains(r));
}

TEST_F(StateTest, SetEmptiness) {
    StateSet set;
    EXPECT_TRUE(set.is_empty());

    set.add(s);
    EXPECT_FALSE(set.is_empty());
}

TEST_F(StateTest, SetSize) {
    StateSet set;
    EXPECT_EQ(0, set.size());
    
    set.add(s);
    EXPECT_EQ(1, set.size());
    
    set.add(p);
    EXPECT_EQ(2, set.size());
    
    set.add(q);
    EXPECT_EQ(2, set.size());
    
    set.remove(q);
    EXPECT_EQ(1, set.size());
}
