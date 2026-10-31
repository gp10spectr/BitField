#include "pch.h"
#include "bit_field.h"

TEST(BitFieldTest, DefaultConstructor) {
    BitField bf;
    EXPECT_EQ(bf.get_size(), 0);
}

TEST(BitFieldTest, SizedConstructor) {
    BitField bf(40);
    EXPECT_EQ(bf.get_size(), 40);
    for (size_t i = 1; i <= 40; ++i) {
        EXPECT_FALSE(bf.get_bit(i));
    }
}

TEST(BitFieldTest, CopyConstructor) {
    BitField bf1(10);
    bf1.set_bit(3);
    bf1.set_bit(7);

    BitField bf2(bf1);
    EXPECT_EQ(bf2.get_size(), 10);
    EXPECT_TRUE(bf2.get_bit(3));
    EXPECT_TRUE(bf2.get_bit(7));
    EXPECT_FALSE(bf2.get_bit(1));
}

TEST(BitFieldTest, MoveConstructor) {
    BitField bf1(10);
    bf1.set_bit(5);

    BitField bf2(std::move(bf1));
    EXPECT_EQ(bf2.get_size(), 10);
    EXPECT_TRUE(bf2.get_bit(5));
    EXPECT_EQ(bf1.get_size(), 0);
}

TEST(BitFieldTest, CopyAssignment) {
    BitField bf1(10);
    bf1.set_bit(2);
    BitField bf2(5);

    bf2 = bf1;
    EXPECT_EQ(bf2.get_size(), 10);
    EXPECT_TRUE(bf2.get_bit(2));

    bf2 = bf2;
    EXPECT_EQ(bf2.get_size(), 10);
    EXPECT_TRUE(bf2.get_bit(2));
}

TEST(BitFieldTest, MoveAssignment) {
    BitField bf1(15);
    bf1.set_bit(12);
    BitField bf2;

    bf2 = std::move(bf1);
    EXPECT_EQ(bf2.get_size(), 15);
    EXPECT_TRUE(bf2.get_bit(12));
    EXPECT_EQ(bf1.get_size(), 0);
}

TEST(BitFieldTest, SetResetGetBit) {
    BitField bf(70); //more than 32 bits to test more words

    bf.set_bit(1);   //first word
    bf.set_bit(32);  //end of word
    bf.set_bit(33);  //second word
    bf.set_bit(70);  //last bit of third word

    EXPECT_TRUE(bf.get_bit(1));
    EXPECT_TRUE(bf.get_bit(32));
    EXPECT_TRUE(bf.get_bit(33));
    EXPECT_TRUE(bf.get_bit(70));
    EXPECT_FALSE(bf.get_bit(2));
    EXPECT_FALSE(bf.get_bit(34));

    bf.reset_bit(33);
    EXPECT_FALSE(bf.get_bit(33));
}

TEST(BitFieldTest, OutOfRangeExceptions) {
    BitField bf(10);

    EXPECT_THROW(bf.get_bit(0), std::out_of_range);
    EXPECT_THROW(bf.set_bit(0), std::out_of_range);
    EXPECT_THROW(bf.reset_bit(0), std::out_of_range);

    EXPECT_THROW(bf.get_bit(11), std::out_of_range);
    EXPECT_THROW(bf.set_bit(11), std::out_of_range);
    EXPECT_THROW(bf.reset_bit(11), std::out_of_range);
}

TEST(BitFieldTest, EqualityOperators) {
    BitField bf1(10);
    BitField bf2(10);
    BitField bf3(12);

    bf1.set_bit(5);
    bf2.set_bit(5);
    bf3.set_bit(5);

    EXPECT_TRUE(bf1 == bf2);
    EXPECT_FALSE(bf1 != bf2);

    EXPECT_FALSE(bf1 == bf3); //diff size
    EXPECT_TRUE(bf1 != bf3);

    bf2.set_bit(6);
    EXPECT_FALSE(bf1 == bf2);
}

TEST(BitFieldTest, BitwiseNOT) {
    BitField bf(5);
    bf.set_bit(1);
    bf.set_bit(3);

    BitField inv = ~bf;
    EXPECT_EQ(inv.get_size(), 5);
    EXPECT_FALSE(inv.get_bit(1));
    EXPECT_TRUE(inv.get_bit(2));
    EXPECT_FALSE(inv.get_bit(3));
    EXPECT_TRUE(inv.get_bit(4));
    EXPECT_TRUE(inv.get_bit(5));
}

TEST(BitFieldTest, BitwiseOR) {
    BitField bf1(5);
    bf1.set_bit(1);
    bf1.set_bit(4);

    BitField bf2(7);
    bf2.set_bit(4);
    bf2.set_bit(6);

    BitField res = bf1 | bf2;
    EXPECT_EQ(res.get_size(), 7); //max size
    EXPECT_TRUE(res.get_bit(1));
    EXPECT_FALSE(res.get_bit(2));
    EXPECT_FALSE(res.get_bit(3));
    EXPECT_TRUE(res.get_bit(4));
    EXPECT_FALSE(res.get_bit(5));
    EXPECT_TRUE(res.get_bit(6));
    EXPECT_FALSE(res.get_bit(7));
}

TEST(BitFieldTest, BitwiseAND) {
    BitField bf1(10);
    bf1.set_bit(2);
    bf1.set_bit(5);

    BitField bf2(8);
    bf2.set_bit(5);
    bf2.set_bit(7);

    BitField res = bf1 & bf2;
    EXPECT_EQ(res.get_size(), 10);
    EXPECT_FALSE(res.get_bit(2));
    EXPECT_TRUE(res.get_bit(5));
    EXPECT_FALSE(res.get_bit(7));
}

TEST(BitFieldTest, StreamInput) {
    BitField bf;
    std::stringstream ss("10110");
    ss >> bf;

    EXPECT_EQ(bf.get_size(), 5);
    EXPECT_TRUE(bf.get_bit(1));
    EXPECT_FALSE(bf.get_bit(2));
    EXPECT_TRUE(bf.get_bit(3));
    EXPECT_TRUE(bf.get_bit(4));
    EXPECT_FALSE(bf.get_bit(5));

    std::stringstream bad_ss("10210");
    bad_ss >> bf;
    EXPECT_TRUE(bad_ss.fail());
}