#include <gtest/gtest.h>
#include <Ircis.h>
#include <fstream>
#include <memory>
#include <sstream>

namespace Ircis {
  class SystemTests : public ::testing::Test {
  protected:
    void SetUp() override {

    }

    // void TearDown() override {}
    std::shared_ptr<std::ostringstream> output = std::make_shared<std::ostringstream>();
    Ircis hello_world_{"hello_world.txt", output, true};
  };

  TEST_F(SystemTests, HelloWorld) {
    while (hello_world_.update());
    ASSERT_EQ("Hello World!\n \n", output->str());
    output->clear();
  }
}
