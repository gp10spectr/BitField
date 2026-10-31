#include "pch.h"
#include "set.h"

TEST(SetTest, DefaultConstructor) {
    Set s;
    EXPECT_EQ(s.get_max(), 0);
}

TEST(SetTest, SizedConstructor) {
    Set s(20);
    EXPECT_EQ(s.get_max(), 20);
    for (size_t i = 1; i <= 20; ++i) {
        EXPECT_FALSE(s.is_member(i));
    }
}

TEST(SetTest, InsertEraseIsMember) {
    Set s(50);
    s.insert(10);
    s.insert(45);

    EXPECT_TRUE(s.is_member(10));
    EXPECT_TRUE(s.is_member(45));
    EXPECT_FALSE(s.is_member(11));

    s.erase(10);
    EXPECT_FALSE(s.is_member(10));
}

TEST(SetTest, OutOfRangeExceptions) {
    Set s(10);
    EXPECT_THROW(s.insert(0), std::out_of_range);
    EXPECT_THROW(s.insert(11), std::out_of_range);
    EXPECT_THROW(s.erase(0), std::out_of_range);
    EXPECT_THROW(s.erase(11), std::out_of_range);
    EXPECT_THROW(s.is_member(0), std::out_of_range);
    EXPECT_THROW(s.is_member(11), std::out_of_range);
}

TEST(SetTest, UnionOperation) {
    Set s1(10);
    s1.insert(2);
    s1.insert(5);

    Set s2(8);
    s2.insert(5);
    s2.insert(7);

    Set s3 = s1 + s2;
    EXPECT_EQ(s3.get_max(), 10);
    EXPECT_TRUE(s3.is_member(2));
    EXPECT_TRUE(s3.is_member(5));
    EXPECT_TRUE(s3.is_member(7));
    EXPECT_FALSE(s3.is_member(1));
}

TEST(SetTest, IntersectionOperation) {
    Set s1(10);
    s1.insert(2);
    s1.insert(5);
    s1.insert(7);

    Set s2(15);
    s2.insert(5);
    s2.insert(7);
    s2.insert(12);

    Set s3 = s1 * s2;
    EXPECT_FALSE(s3.is_member(2));
    EXPECT_TRUE(s3.is_member(5));
    EXPECT_TRUE(s3.is_member(7));
    EXPECT_FALSE(s3.is_member(12));
}

TEST(SetTest, DifferenceOperation) {
    Set s1(10);
    s1.insert(1);
    s1.insert(2);
    s1.insert(3);

    Set s2(10);
    s2.insert(2);
    s2.insert(4);

    Set s3 = s1 - s2; // {1, 3}
    EXPECT_TRUE(s3.is_member(1));
    EXPECT_FALSE(s3.is_member(2));
    EXPECT_TRUE(s3.is_member(3));
    EXPECT_FALSE(s3.is_member(4));
}

TEST(SetTest, NegationOperation) {
    Set s(4);
    s.insert(2);

    Set neg = -s;
    EXPECT_EQ(neg.get_max(), 4);
    EXPECT_TRUE(neg.is_member(1));
    EXPECT_FALSE(neg.is_member(2));
    EXPECT_TRUE(neg.is_member(3));
    EXPECT_TRUE(neg.is_member(4));
}

TEST(SetTest, CompoundAssignmentOperators) {
    Set s1(10);
    s1.insert(2);
    Set s2(10);
    s2.insert(3);

    s1 += s2;
    EXPECT_TRUE(s1.is_member(2));
    EXPECT_TRUE(s1.is_member(3));

    s1 *= s2;
    EXPECT_FALSE(s1.is_member(2));
    EXPECT_TRUE(s1.is_member(3));

    s1 -= s2;
    EXPECT_FALSE(s1.is_member(3));
}

TEST(SetTest, CastToBitField) {
    Set s(10);
    s.insert(5);

    BitField bf = static_cast<BitField>(s);
    EXPECT_EQ(bf.get_size(), 10);
    EXPECT_TRUE(bf.get_bit(5));
    EXPECT_FALSE(bf.get_bit(4));
}

TEST(SetTest, StreamOutput) {
    Set s(5);
    s.insert(1);
    s.insert(4);

    std::stringstream ss;
    ss << s;
    EXPECT_EQ(ss.str(), "{ 1, 4 }");

    Set empty_s;
    std::stringstream empty_ss;
    empty_ss << empty_s;
    EXPECT_EQ(empty_ss.str(), "{  }");
}