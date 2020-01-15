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
  };

  TEST_F(VariableTests, LocalVarTest) {
    while (local_var_.update());
    ASSERT_EQ("4", getline(*output_));
    output_->clear();
  }

  std::string getline(std::stringstream& os) {
    std::string line;
    std::getline(os, line);
    return line;
  }
}
