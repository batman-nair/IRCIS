#include <gtest/gtest.h>
#include <Ircis.h>
#include <fstream>

namespace Ircis {
  std::string getline(std::stringstream& os);

  class VariableTests : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}

    std::shared_ptr<std::stringstream> output_ = std::make_shared<std::stringstream>();
    Ircis local_var_{"local_var_test.txt", output_, true};
    Ircis local_var_split_{"local_var_split_test.txt", output_, true};
    Ircis local_var_multi_{"local_var_multi.txt", output_, true};
  };

  TEST_F(VariableTests, LocalVarTest) {
    while (local_var_.update());
    ASSERT_EQ("4", getline(*output_));
    output_->clear();
  }

  TEST_F(VariableTests, LocalVarSplitTest) {
    while (local_var_split_.update());
    ASSERT_EQ("44", getline(*output_));
    ASSERT_EQ("44", getline(*output_));
    output_->clear();
  }

  TEST_F(VariableTests, LocalVarMultiTest) {
    while (local_var_multi_.update());
    ASSERT_EQ("22", getline(*output_));
    ASSERT_EQ("24", getline(*output_));
    output_->clear();
  }


  std::string getline(std::stringstream& os) {
    std::string line;
    std::getline(os, line);
    return line;
  }
}
