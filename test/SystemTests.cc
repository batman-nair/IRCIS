#include <gtest/gtest.h>
#include <Ircis.h>
#include <fstream>
#include <memory>
#include <sstream>

namespace Ircis {
  std::string getline(std::stringstream& os);

  class SystemTests : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}

    std::shared_ptr<std::stringstream> output_ = std::make_shared<std::stringstream>();
    Ircis hello_world_{"hello_world.txt", output_, true};
    Ircis calc_{"calc.txt", output_, true};
    Ircis factors_{"factors.txt", output_, true};
    Ircis num2bin_{"num2bin.txt", output_, true};
    Ircis options_{"options.txt", output_, true};
    Ircis options_n_{"options.txt", 49, 21, 'N', output_, true};
    Ircis options_s_{"options.txt", 2, 2, 'S', output_, true};
    Ircis options_w_{"options.txt", 59, 12, 'W', output_, true};
    Ircis random_{"random.txt", output_, true};
  };

  TEST_F(SystemTests, HelloWorldOutput) {
    while (hello_world_.update());
    ASSERT_EQ("Hello World!", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, CalcOutput) {
    while (calc_.update());
    ASSERT_EQ("+=50", getline(*output_));
    ASSERT_EQ("-=10", getline(*output_));
    ASSERT_EQ("*=600", getline(*output_));
    ASSERT_EQ("/=1", getline(*output_));
    ASSERT_EQ("%=10", getline(*output_));
    ASSERT_EQ("20 30", getline(*output_));
    ASSERT_EQ("^=1953125", getline(*output_));
    ASSERT_EQ("&=1", getline(*output_));
    ASSERT_EQ("|=13", getline(*output_));
    ASSERT_EQ("V=12", getline(*output_));
    ASSERT_EQ("<=2560", getline(*output_));
    ASSERT_EQ(">=0", getline(*output_));
    ASSERT_EQ("9 5", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, FactorsOutput) {
    while (factors_.update());
    ASSERT_EQ(" 1 2 4 5 10 20 ", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, Num2BinOutput) {
    while (num2bin_.update());
    ASSERT_EQ("0111 ", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, OptionsOutput) {
    while (options_.update());
    ASSERT_EQ("This starts with the default of (0, 0) and E.", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, OptionsNOutput) {
    while (options_n_.update());
    ASSERT_EQ("This starts with (49, 21) and N.", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, OptionsSOutput) {
    while (options_s_.update());
    ASSERT_EQ("This starts with (2, 2) and S.", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, OptionsWOutput) {
    while (options_w_.update());
    ASSERT_EQ("This starts with (59, 12) and W.", getline(*output_));
    output_->clear();
  }
  TEST_F(SystemTests, RandomOutput) {
    while (random_.update());
    ASSERT_EQ(2, getline(*output_).size()); // How else to test random numbers? :/
    output_->clear();
  }

}
