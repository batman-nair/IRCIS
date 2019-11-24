#include <gtest/gtest.h>
#include <DataType.h>

namespace Ircis {
  class DataTypeTest : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}

    DataType d_;
    DataType num1_{4, true};
    DataType num2_{2, true};
  };

  TEST_F(DataTypeTest, IsZeroInit) {
    EXPECT_EQ(0, d_.value);
    EXPECT_STREQ("0", d_.to_string().c_str());
  }

  TEST_F(DataTypeTest, CharTest) {
    d_ = DataType('c');
    EXPECT_STREQ("c", d_.to_string().c_str());
  }

  TEST_F(DataTypeTest, ArithTest) {
    EXPECT_EQ(6, (num1_+num2_).value);
    EXPECT_EQ(2, (num1_-num2_).value);
    EXPECT_EQ(8, (num1_*num2_).value);
    EXPECT_EQ(2, (num1_/num2_).value);
    EXPECT_EQ(0, (num1_%num2_).value);
  }
}
