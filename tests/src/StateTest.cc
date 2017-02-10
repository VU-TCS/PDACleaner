#include <gtest/gtest.h>

#include "State.h"

class StateTest : public ::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}
};

TEST(PairStateTest, Equality) {
    SimpleState s(1);
    SimpleState p(2);
    SimpleState q(1);

    PairState pair_1(&s, &p);
    PairState pair_2(&p, &s);
    PairState pair_3(&q, &p);

    EXPECT_FALSE(pair_1.equals(pair_2));
    EXPECT_TRUE(pair_1.equals(pair_3));
}

TEST(StateSetTest, Emptiness) {
    StateSet set;
    EXPECT_TRUE(set.is_empty());

    SimpleState s(1);;
    set.add(&s);
    EXPECT_FALSE(set.is_empty());
}

TEST(StateSetTest, Size) {
    StateSet set;
    EXPECT_EQ(0, set.size());

    SimpleState s(1);
    SimpleState p(2);
    SimpleState q(1);
    
    set.add(&s);
    EXPECT_EQ(1, set.size());
    
    set.add(&p);
    EXPECT_EQ(2, set.size());
    
    set.add(&q);
    EXPECT_EQ(2, set.size());
    
    set.remove(&q);
    EXPECT_EQ(1, set.size());
}
