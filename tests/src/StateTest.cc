#include <gtest/gtest.h>

#include "State.h"

class StateTest : public ::testing::Test {
    protected:
        virtual void SetUp() {}
        virtual void TearDown() {}
};

TEST(StateSetTest, Emptiness) {
    StateSet set;
    EXPECT_TRUE(set.is_empty());

    State s(1);;
    set.add(s);
    EXPECT_FALSE(set.is_empty());
}

TEST(StateSetTest, Size) {
    StateSet set;
    EXPECT_EQ(0, set.size());

    State s(1);
    State p(2);
    State q(1);
    
    set.add(s);
    EXPECT_EQ(1, set.size());
    
    set.add(p);
    EXPECT_EQ(2, set.size());
    
    set.add(q);
    EXPECT_EQ(2, set.size());
    
    set.remove(q);
    EXPECT_EQ(1, set.size());
}
